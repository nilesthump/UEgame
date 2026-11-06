#include "experiment/Session/ExperimentSession.h"
#include <cmath>

namespace experiment {
ExperimentSession::ExperimentSession(RunId runId) noexcept { state_.runId = runId; }
bool ExperimentSession::NotifyWorldReady(const WorldReadyFact& fact) noexcept {
    if (state_.runId == 0 || fact.runId != state_.runId ||
        state_.phase != ExperimentPhase::Preparing ||
        !std::isfinite(fact.occurredAt.engineWorldSeconds) ||
        !std::isfinite(fact.occurredAt.realSeconds) ||
        fact.occurredAt.engineWorldSeconds < 0 || fact.occurredAt.realSeconds < 0) {
        return false;
    }
    state_.worldStartedAt = fact.occurredAt;
    state_.phase = ExperimentPhase::Rescue;
    return true;
}
ExperimentViewProjection ExperimentSession::View() const noexcept {
    return {state_.runId, state_.phase, state_.worldStartedAt};
}
}
