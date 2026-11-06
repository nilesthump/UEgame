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
enum class ExperimentPhase { Preparing, Rescue };
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
        bool swordUsed{};
        bool magicUsed{};
    } state_;
    // Rescue-local deduplication and usage metadata, not a world-state copy.
    std::unordered_set<std::uint64_t> acceptedSequences_;
    std::unordered_set<ActionId> directedActions_;
    std::vector<ActionUsedFact> usedActions_;
};
}
