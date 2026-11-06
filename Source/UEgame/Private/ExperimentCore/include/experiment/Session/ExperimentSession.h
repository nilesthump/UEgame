#pragma once
#include <cstdint>
#include <optional>

namespace experiment {
using RunId = std::uint64_t;
struct ExperimentMoment {
    double engineWorldSeconds{};
    double realSeconds{};
};
enum class ExperimentPhase { Preparing, Rescue };
struct WorldReadyFact {
    RunId runId{};
    ExperimentMoment occurredAt{};
};
struct ExperimentViewProjection {
    RunId runId{};
    ExperimentPhase phase{ExperimentPhase::Preparing};
    std::optional<ExperimentMoment> worldStartedAt;
};
// The only owner of experimental state; world readiness is supplied by the host.
class ExperimentSession final {
public:
    explicit ExperimentSession(RunId runId) noexcept;
    bool NotifyWorldReady(const WorldReadyFact& fact) noexcept;
    [[nodiscard]] ExperimentViewProjection View() const noexcept;
private:
    struct ExperimentRunState {
        RunId runId{};
        ExperimentPhase phase{ExperimentPhase::Preparing};
        std::optional<ExperimentMoment> worldStartedAt;
    } state_;
};
}
