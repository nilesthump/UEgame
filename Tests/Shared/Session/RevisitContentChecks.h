#pragma once
#include "SettlementChecks.h"
#include <algorithm>

namespace experiment::checks {
inline bool RevisitContentReceipts() {
    using Id = RevisitContentId;
    struct Branch { RescueMethod method; Id crowd; Id teacher; };
    constexpr Branch branches[] = {{RescueMethod::Sword, Id::CrowdSword, Id::TeacherSword},
        {RescueMethod::Magic, Id::CrowdMagic, Id::TeacherMagic},
        {RescueMethod::Mixed, Id::CrowdMixed, Id::TeacherMixed}};
    constexpr RunId run = 71;
    for (const auto& branch : branches) for (bool teacherFirst : {false, true}) {
        auto session = ReadySession(run, false);
        session.NotifyThreatDiscovered({{run, 1, {1, 1}}});
        if (branch.method != RescueMethod::Magic)
            session.NotifyActionUsed({{run, 2, {2, 2}}, 2, ActionKind::Sword});
        if (branch.method != RescueMethod::Sword)
            session.NotifyActionUsed({{run, 3, {3, 3}}, 3, ActionKind::Magic});
        auto first = branch.crowd;
        auto second = branch.teacher;
        if (teacherFirst) std::swap(first, second);
        const auto deliver = [&](Id id, std::uint64_t generation, const FactContext& context) {
            return session.NotifyRevisitContentDelivered(context, id, generation);
        };
        const FactContext firstContext{run, 10, {24, 34}};
        const FactContext secondContext{run, 11, {26, 36}};
        if (deliver(first, 1, firstContext).accepted) return false;
        const auto success = Settle(session, 4, 1, true, true, true);
        if (success.commands.size() != 2 || !session.TryConsumeEffect(success.commands[0]) ||
            !session.TryConsumeEffect(success.commands[1])) return false;
        const auto token = success.commands[1].requestId;
        session.NotifySkipReady({run, 5, {12, 22}}, token, SkipMilestone::EnvironmentSettled);
        session.NotifySkipReady({run, 6, {14, 24}}, token, SkipMilestone::TownPrepared);
        const auto restore = session.NotifySkipReady({run, 7, {16, 26}}, token, SkipMilestone::TransitionFinished);
        if (restore.commands.size() != 1 || !session.TryConsumeEffect(restore.commands[0]) ||
            deliver(first, 1, firstContext).accepted ||
            !session.NotifyControlRestored({run, 8, {20, 30}}, token)) return false;
        for (const auto& other : branches)
            if (other.method != branch.method &&
                (deliver(other.crowd, 1, firstContext).accepted || deliver(other.teacher, 1, firstContext).accepted))
                return false;
        if (deliver(static_cast<Id>(4), 1, firstContext).accepted || deliver(first, 0, firstContext).accepted ||
            deliver(first, 1, {run + 1, 10, {24, 34}}).accepted ||
            deliver(first, 1, {run, 8, {24, 34}}).accepted ||
            deliver(first, 1, {run, 10, {24, 29}}).accepted) return false;
        const auto progress = deliver(first, 2, firstContext);
        const auto view = session.View();
        if (!progress.accepted || !progress.commands.empty() || view.phase != ExperimentPhase::Revisit ||
            view.teacherRecallDelivered != teacherFirst || view.crowdContentDelivered == teacherFirst ||
            view.revisitCompletedAt || deliver(first, 3, secondContext).accepted ||
            deliver(second, 2, secondContext).accepted || deliver(second, 3, firstContext).accepted) return false;
        const auto finish = deliver(second, 3, secondContext);
        const auto result = session.View();
        if (!finish.accepted || finish.commands.size() != 1 || finish.commands[0].kind != EffectKind::StopRun ||
            !session.TryConsumeEffect(finish.commands[0]) || session.TryConsumeEffect(finish.commands[0]) ||
            result.phase != ExperimentPhase::Ended || !result.teacherRecallDelivered || !result.crowdContentDelivered ||
            !result.revisitCompletedAt || result.revisitCompletedAt->realSeconds != 36 ||
            deliver(second, 4, {run, 12, {27, 37}}).accepted) return false;
    }
    return true;
}
}
