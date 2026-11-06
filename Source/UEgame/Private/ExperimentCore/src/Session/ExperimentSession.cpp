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
    return acceptedSequences_.insert(context.sequence).second;
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
