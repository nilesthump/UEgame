#pragma once
#include <cstdint>
#include <optional>
#include <unordered_set>
#include <vector>

namespace experiment {
using RunId = std::uint64_t;
using ActionId = std::uint64_t;
struct ExperimentMoment {
    double engineWorldSeconds{};
    double realSeconds{};
};
enum class ExperimentPhase { Preparing, Rescue, TimeSkip, Ended };
struct WorldReadyFact {
    RunId runId{};
    ExperimentMoment occurredAt{};
};
struct FactContext {
    RunId runId{};
    std::uint64_t sequence{};
    ExperimentMoment occurredAt{};
};
struct ThreatDiscoveredFact { FactContext context; };
enum class ActionKind { Sword, Magic };
struct ActionUsedFact {
    FactContext context;
    ActionId actionId{};
    ActionKind kind{};
};
struct AttackDirectedAtGoblinFact {
    FactContext context;
    ActionId actionId{};
};
using BatchId = std::uint64_t;
struct AftermathObservedFact { FactContext context; };
// Host asserts all required facts and damage in this batch have been delivered.
// Missing entities must produce valid=false, never implicit death.
struct EncounterSnapshot {
    RunId runId{};
    BatchId batchId{};
    bool valid{};
    bool playerAlive{};
    bool childAlive{};
    bool allFiveThreatsDead{};
};
struct SettlementCompletedFact {
    FactContext context;
    BatchId batchId{};
    bool allFactsDelivered{};
};
enum class RescueDecision { Continue, AwaitAftermath, Failed, Succeeded, MissingMethod };
enum class RescueMethod { Sword, Magic, Mixed };
struct ExperimentFlowRules final {
    static RescueDecision EvaluateRescue(const EncounterSnapshot& snapshot,
        bool threatObserved, bool aftermathObserved, bool hasMethod) noexcept;
};
enum class EffectKind { RequestStableSnapshot, StopRescue, BeginTimeSkip, StopRun };
struct ExperimentEffect {
    RunId runId{};
    ExperimentPhase phase{};
    EffectKind kind{};
};
struct ExperimentStepEffects {
    bool accepted{};
    RescueDecision decision{RescueDecision::Continue};
    std::vector<ExperimentEffect> commands;
};
struct RescueResult {
    bool succeeded{};
    bool playerDied{};
    bool childDied{};
    std::optional<RescueMethod> method;
    ExperimentMoment closedAt{};
    std::optional<double> elapsedRealSeconds;
};
struct ExperimentViewProjection {
    RunId runId{};
    ExperimentPhase phase{ExperimentPhase::Preparing};
    std::optional<ExperimentMoment> worldStartedAt;
    std::optional<ExperimentMoment> threatDiscoveredAt;
    std::optional<ExperimentMoment> firstAttackAt;
    bool swordUsed{};
    bool magicUsed{};
};
// The only owner of experimental state; world readiness is supplied by the host.
class ExperimentSession final {
public:
    explicit ExperimentSession(RunId runId) noexcept;
    bool NotifyWorldReady(const WorldReadyFact& fact) noexcept;
    bool NotifyThreatDiscovered(const ThreatDiscoveredFact& fact);
    bool NotifyActionUsed(const ActionUsedFact& fact);
    bool NotifyAttackAtGoblin(const AttackDirectedAtGoblinFact& fact);
    ExperimentStepEffects NotifyAftermathObserved(const AftermathObservedFact& fact);
    ExperimentStepEffects NotifyBatchSettled(const SettlementCompletedFact& fact,
        const EncounterSnapshot& snapshot);
    [[nodiscard]] std::optional<RescueResult> RescueOutcome() const noexcept { return state_.rescueResult; }
    [[nodiscard]] ExperimentViewProjection View() const noexcept;
private:
    bool AcceptContext(const FactContext& context);
    void RefreshUsage() noexcept;
    struct ExperimentRunState {
        RunId runId{};
        ExperimentPhase phase{ExperimentPhase::Preparing};
        std::optional<ExperimentMoment> worldStartedAt;
        std::optional<FactContext> discovery;
        std::optional<FactContext> firstAttack;
        bool aftermathObserved{};
        std::optional<RescueResult> rescueResult;
        bool swordUsed{};
        bool magicUsed{};
    } state_;
    // Rescue-local deduplication and usage metadata, not a world-state copy.
    std::unordered_set<std::uint64_t> acceptedSequences_;
    std::unordered_set<ActionId> directedActions_;
    std::vector<ActionUsedFact> usedActions_;
    BatchId lastSettledBatch_{};
    std::uint64_t latestFactSequence_{};
    std::optional<ExperimentMoment> latestFactMoment_;
    void ClearRescueCaches();
};
}
