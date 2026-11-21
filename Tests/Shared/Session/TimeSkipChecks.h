#pragma once
#include "SettlementChecks.h"
#include <algorithm>
#include <array>

namespace experiment::checks {
inline bool TimeSkipReceipts() {
    const RunId run = 47;
    const FactContext restore{run, 11, {19, 32}};
    std::array milestones{SkipMilestone::EnvironmentSettled, SkipMilestone::TownPrepared,
        SkipMilestone::TransitionFinished};
    // Independent receipts may arrive in any order, including a late earlier occurrence.
    const std::array<FactContext, 3> receipts{{{run, 6, {12, 23}}, {run, 7, {14, 25}}, {run, 8, {16, 28}}}};
    do {
        auto session = ReadySession(run);
        const auto success = Settle(session, 4, 1, true, true, true);
        if (success.commands.size() != 2) return false;
        const auto& begin = success.commands[1];
        const auto token = begin.requestId;
        if (session.NotifySkipReady(receipts[0], token, milestones[0]).accepted ||
            !session.TryConsumeEffect(success.commands[0]) || !session.TryConsumeEffect(begin)) return false;
        if (session.NotifySkipReady({run + 1, 6, {12, 23}}, token, milestones[0]).accepted ||
            session.NotifySkipReady(receipts[0], token + 1, milestones[0]).accepted ||
            session.NotifySkipReady({run, 6, {9, 19}}, token, milestones[0]).accepted) return false;
        ExperimentEffect restoreRequest;
        for (std::size_t i = 0; i < milestones.size(); ++i) {
            const auto milestone = milestones[i];
            const auto& receipt = receipts[static_cast<std::size_t>(milestone)];
            if (session.NotifyControlRestored(restore, token)) return false;
            const auto progress = session.NotifySkipReady(receipt, token, milestone);
            if (!progress.accepted || session.View().phase != ExperimentPhase::TimeSkip ||
                session.View().revisitStartedAt || session.NotifySkipReady(receipt, token, milestone).accepted ||
                session.NotifySkipReady({run, 9, {18, 30}}, token, milestone).accepted) return false;
            if (i + 1 < milestones.size()) {
                if (!progress.commands.empty() || session.NotifySkipReady(receipt, token, milestones[i + 1]).accepted)
                    return false;
            } else {
                if (progress.commands.size() != 1 || progress.commands[0].kind != EffectKind::RestoreControl) return false;
                restoreRequest = progress.commands[0];
            }
        }
        if (session.NotifyControlRestored(restore, token) || !session.TryConsumeEffect(restoreRequest) ||
            session.NotifyControlRestored(restore, token + 1) ||
            session.NotifyControlRestored({run + 1, 11, {19, 32}}, token) ||
            session.NotifyControlRestored({run, 11, {15, 27}}, token)) return false;
        if (!session.NotifyControlRestored(restore, token) ||
            session.NotifySkipReady(receipts[0], token, milestones[0]).accepted ||
            session.NotifyControlRestored({run, 12, {20, 33}}, token)) return false;
        const auto view = session.View();
        if (view.phase != ExperimentPhase::Revisit || !view.revisitStartedAt ||
            view.revisitStartedAt->engineWorldSeconds != restore.occurredAt.engineWorldSeconds ||
            view.revisitStartedAt->realSeconds != restore.occurredAt.realSeconds) return false;
    } while (std::next_permutation(milestones.begin(), milestones.end()));

    auto cancelled = ReadySession(run);
    const auto success = Settle(cancelled, 4, 1, true, true, true);
    if (success.commands.size() != 2 || !cancelled.TryConsumeEffect(success.commands[0]) ||
        !cancelled.TryConsumeEffect(success.commands[1])) return false;
    const auto token = success.commands[1].requestId;
    cancelled.InvalidatePendingEffects();
    return !cancelled.NotifySkipReady(receipts[0], token, SkipMilestone::EnvironmentSettled).accepted &&
        !cancelled.NotifyControlRestored(restore, token) && cancelled.View().phase == ExperimentPhase::TimeSkip;
}
}
