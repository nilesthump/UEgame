#pragma once
#include "experiment/Session/ExperimentSession.h"
#include <limits>

namespace experiment::checks {
inline bool WorldReadyBoundary() {
    ExperimentSession session{1};
    if (session.View().worldStartedAt || session.View().phase != ExperimentPhase::Preparing) return false;
    if (session.NotifyWorldReady({2, {0, 0}})) return false;
    if (session.NotifyWorldReady({1, {0, std::numeric_limits<double>::infinity()}})) return false;
    if (session.NotifyWorldReady({1, {-1, 0}})) return false;
    if (!session.NotifyWorldReady({1, {0, 0}})) return false;
    if (session.NotifyWorldReady({1, {8, 12}})) return false;
    const auto view = session.View();
    if (view.phase != ExperimentPhase::Rescue || !view.worldStartedAt ||
        view.worldStartedAt->realSeconds != 0 || view.worldStartedAt->engineWorldSeconds != 0) return false;
    ExperimentSession next{2};
    if (next.NotifyWorldReady({1, {1, 1}}) || next.View().worldStartedAt) return false;
    ExperimentSession invalid{0};
    return !invalid.NotifyWorldReady({0, {0, 0}});
}
}
