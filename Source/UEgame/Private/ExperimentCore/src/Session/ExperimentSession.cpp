#include "experiment/Session/ExperimentSession.h"
#include <algorithm>
#include <cmath>

namespace experiment {
namespace {
bool ValidMoment(const ExperimentMoment& moment) noexcept {
    return std::isfinite(moment.engineWorldSeconds) && std::isfinite(moment.realSeconds) &&
        moment.engineWorldSeconds >= 0 && moment.realSeconds >= 0;
}
}
ExperimentSession::ExperimentSession(RunId runId) noexcept { state_.runId = runId; }
ExperimentEffect ExperimentSession::IssueEffect(EffectKind kind) {
    const ExperimentEffect effect{state_.runId, state_.phase, kind, nextRequestId_++};
    pendingEffects_.push_back(effect);
    return effect;
}
void ExperimentSession::InvalidatePendingEffects() noexcept { pendingEffects_.clear(); }
bool ExperimentSession::TryConsumeEffect(const ExperimentEffect& effect) {
    if (effect.runId != state_.runId || effect.phase != state_.phase ||
        pendingEffects_.empty() || pendingEffects_.front() != effect) return false;
    pendingEffects_.erase(pendingEffects_.begin());
    return true;
}
bool ExperimentSession::NotifyWorldReady(const WorldReadyFact& fact) noexcept {
    if (state_.runId == 0 || fact.runId != state_.runId ||
        state_.phase != ExperimentPhase::Preparing || !ValidMoment(fact.occurredAt)) return false;
    state_.worldStartedAt = fact.occurredAt;
    state_.phase = ExperimentPhase::Rescue;
    return true;
}
bool ExperimentSession::AcceptContext(const FactContext& context) {
    if (context.runId != state_.runId || context.sequence == 0 ||
        state_.phase != ExperimentPhase::Rescue || !ValidMoment(context.occurredAt) ||
        context.occurredAt.engineWorldSeconds < state_.worldStartedAt->engineWorldSeconds ||
        context.occurredAt.realSeconds < state_.worldStartedAt->realSeconds) return false;
    if (!acceptedSequences_.insert(context.sequence).second) return false;
    latestFactSequence_ = std::max(latestFactSequence_, context.sequence);
    if (!latestFactMoment_) latestFactMoment_ = context.occurredAt;
    latestFactMoment_->engineWorldSeconds = std::max(latestFactMoment_->engineWorldSeconds, context.occurredAt.engineWorldSeconds);
    latestFactMoment_->realSeconds = std::max(latestFactMoment_->realSeconds, context.occurredAt.realSeconds);
    return true;
}
bool ExperimentSession::NotifyThreatDiscovered(const ThreatDiscoveredFact& fact) {
    if (!AcceptContext(fact.context)) return false;
    if (!state_.discovery || fact.context.sequence < state_.discovery->sequence) {
        state_.discovery = fact.context;
        RefreshUsage();
    }
    return true;
}
bool ExperimentSession::NotifyActionUsed(const ActionUsedFact& fact) {
    if (fact.actionId == 0 || (fact.kind != ActionKind::Sword && fact.kind != ActionKind::Magic) ||
        std::any_of(usedActions_.begin(), usedActions_.end(),
            [&](const auto& used) { return used.actionId == fact.actionId; }) ||
        !AcceptContext(fact.context)) return false;
    usedActions_.push_back(fact);
    RefreshUsage();
    return true;
}
bool ExperimentSession::NotifyAttackAtGoblin(const AttackDirectedAtGoblinFact& fact) {
    if (fact.actionId == 0 || directedActions_.contains(fact.actionId) ||
        !AcceptContext(fact.context)) return false;
    directedActions_.insert(fact.actionId);
    if (!state_.firstAttack || fact.context.sequence < state_.firstAttack->sequence)
        state_.firstAttack = fact.context;
    return true;
}
void ExperimentSession::RefreshUsage() noexcept {
    state_.swordUsed = false;
    state_.magicUsed = false;
    if (!state_.discovery) return;
    for (const auto& action : usedActions_) {
        // Sequence expresses occurrence order, even for equal clock samples or late delivery.
        if (action.context.sequence <= state_.discovery->sequence) continue;
        state_.swordUsed |= action.kind == ActionKind::Sword;
        state_.magicUsed |= action.kind == ActionKind::Magic;
    }
}
ExperimentViewProjection ExperimentSession::View() const noexcept {
    return {state_.runId, state_.phase, state_.worldStartedAt,
        state_.discovery ? std::optional{state_.discovery->occurredAt} : std::nullopt,
        state_.firstAttack ? std::optional{state_.firstAttack->occurredAt} : std::nullopt,
        state_.swordUsed, state_.magicUsed};
}
}

namespace experiment {
RescueDecision ExperimentFlowRules::EvaluateRescue(const EncounterSnapshot& snapshot,
    bool threatObserved, bool aftermathObserved, bool hasMethod) noexcept {
    if (!snapshot.valid) return RescueDecision::Continue;
    if (!snapshot.playerAlive) return RescueDecision::Failed;
    if (!snapshot.childAlive)
        return threatObserved || aftermathObserved ? RescueDecision::Failed : RescueDecision::AwaitAftermath;
    if (!snapshot.allFiveThreatsDead) return RescueDecision::Continue;
    return hasMethod ? RescueDecision::Succeeded : RescueDecision::MissingMethod;
}
ExperimentStepEffects ExperimentSession::NotifyAftermathObserved(const AftermathObservedFact& fact) {
    if (!AcceptContext(fact.context)) return {};
    state_.aftermathObserved = true;
    return {true, RescueDecision::Continue,
        {IssueEffect(EffectKind::RequestStableSnapshot)}};
}
ExperimentStepEffects ExperimentSession::NotifyBatchSettled(const SettlementCompletedFact& fact,
    const EncounterSnapshot& snapshot) {
    // Batch IDs are monotonic stable commit points, unlike independently delivered fact sequences.
    if (!fact.allFactsDelivered || !snapshot.valid || fact.batchId == 0 ||
        fact.batchId <= lastSettledBatch_ || snapshot.batchId != fact.batchId ||
        snapshot.runId != fact.context.runId || fact.context.sequence <= latestFactSequence_ ||
        (latestFactMoment_ && (fact.context.occurredAt.realSeconds < latestFactMoment_->realSeconds ||
            fact.context.occurredAt.engineWorldSeconds < latestFactMoment_->engineWorldSeconds)) ||
        !AcceptContext(fact.context)) return {};
    lastSettledBatch_ = fact.batchId;
    const auto decision = ExperimentFlowRules::EvaluateRescue(snapshot,
        state_.discovery.has_value(), state_.aftermathObserved, state_.swordUsed || state_.magicUsed);
    ExperimentStepEffects effects{true, decision, {}};
    if (decision != RescueDecision::Failed && decision != RescueDecision::Succeeded) return effects;
    RescueResult result;
    result.succeeded = decision == RescueDecision::Succeeded;
    result.playerDied = !snapshot.playerAlive;
    result.childDied = !snapshot.childAlive;
    result.closedAt = fact.context.occurredAt;
    if (state_.firstAttack) result.elapsedRealSeconds = result.closedAt.realSeconds - state_.firstAttack->occurredAt.realSeconds;
    if (result.succeeded) result.method = state_.swordUsed
        ? (state_.magicUsed ? RescueMethod::Mixed : RescueMethod::Sword) : RescueMethod::Magic;
    state_.rescueResult = result;
    state_.phase = result.succeeded ? ExperimentPhase::TimeSkip : ExperimentPhase::Ended;
    ClearRescueCaches();
    InvalidatePendingEffects();
    effects.commands.push_back(IssueEffect(EffectKind::StopRescue));
    effects.commands.push_back(IssueEffect(result.succeeded ? EffectKind::BeginTimeSkip : EffectKind::StopRun));
    return effects;
}
void ExperimentSession::ClearRescueCaches() {
    std::unordered_set<std::uint64_t>{}.swap(acceptedSequences_);
    std::unordered_set<ActionId>{}.swap(directedActions_);
    std::vector<ActionUsedFact>{}.swap(usedActions_);
    lastSettledBatch_ = 0;
    latestFactSequence_ = 0;
    latestFactMoment_.reset();
}
}
