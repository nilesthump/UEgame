#pragma once
#include "experiment/Session/ExperimentSession.h"
namespace experiment::checks {
inline ExperimentSession ReadySession(RunId id, bool method = true) {
    ExperimentSession s{id};
    s.NotifyWorldReady({id, {0, 0}});
    if (method) {
        s.NotifyThreatDiscovered({{id, 1, {1, 1}}});
        s.NotifyActionUsed({{id, 2, {2, 2}}, 1, ActionKind::Sword});
        s.NotifyAttackAtGoblin({{id, 3, {2, 2}}, 1});
    }
    return s;
}
inline ExperimentStepEffects Settle(ExperimentSession& s, std::uint64_t sequence,
    BatchId batch, bool player, bool child, bool cleared, bool complete = true, bool valid = true) {
    return s.NotifyBatchSettled({{s.View().runId, sequence, {10, 20}}, batch, complete},
        {s.View().runId, batch, valid, player, child, cleared});
}
inline bool StableBatchDeathPriority() {
    for (bool player : {false, true}) for (bool child : {false, true}) {
        auto s = ReadySession(1);
        // Neither incomplete nor invalid snapshots may commit or consume the batch identity.
        if (Settle(s, 4, 1, true, true, true, false).accepted ||
            Settle(s, 4, 1, true, true, true, true, false).accepted || s.RescueOutcome()) return false;
        const auto e = Settle(s, 4, 1, player, child, true);
        const auto r = s.RescueOutcome();
        const bool success = player && child;
        if (!e.accepted || !r || r->succeeded != success || r->playerDied == player ||
            r->childDied == child || r->elapsedRealSeconds != 18 || e.commands.size() != 2 ||
            e.commands[0].kind != EffectKind::StopRescue ||
            s.View().phase != (success ? ExperimentPhase::TimeSkip : ExperimentPhase::Ended)) return false;
        if (Settle(s, 5, 2, true, true, true).accepted ||
            s.NotifyActionUsed({{1, 6, {11, 21}}, 2, ActionKind::Magic}) ||
            s.NotifyAftermathObserved({{1, 7, {11, 21}}}).accepted ||
            s.RescueOutcome()->succeeded != success || s.View().magicUsed) return false;
    }
    auto s = ReadySession(2);
    if (s.NotifyBatchSettled({{2, 4, {10, 20}}, 1, true}, {3, 1, true, true, true, true}).accepted ||
        s.NotifyBatchSettled({{2, 4, {10, 20}}, 1, true}, {2, 2, true, true, true, true}).accepted ||
        s.NotifyBatchSettled({{2, 4, {1, 1}}, 1, true}, {2, 1, true, true, true, true}).accepted) return false;
    if (!Settle(s, 4, 2, true, true, false).accepted ||
        Settle(s, 5, 1, true, true, true).accepted || Settle(s, 5, 2, true, true, true).accepted) return false;
    return Settle(s, 5, 3, true, true, true).decision == RescueDecision::Succeeded;
}
inline bool AftermathAndMissingMethod() {
    for (bool observationFirst : {false, true}) {
        auto s = ReadySession(3, false);
        if (!observationFirst && Settle(s, 1, 1, true, false, true).decision != RescueDecision::AwaitAftermath) return false;
        if (s.RescueOutcome() || s.View().phase != ExperimentPhase::Rescue) return false;
        const auto e = s.NotifyAftermathObserved({{3, 2, {11, 21}}});
        if (!e.accepted || e.commands.size() != 1 || e.commands[0].kind != EffectKind::RequestStableSnapshot ||
            s.NotifyAftermathObserved({{3, 2, {11, 21}}}).accepted || s.View().threatDiscoveredAt) return false;
        const auto settled = s.NotifyBatchSettled({{3, 3, {11, 21}}, 2, true}, {3, 2, true, true, false, true});
        if (settled.decision != RescueDecision::Failed || !s.RescueOutcome() ||
            s.RescueOutcome()->elapsedRealSeconds || s.RescueOutcome()->method) return false;
    }
    auto empty = ReadySession(4, false);
    if (Settle(empty, 1, 1, true, true, true).decision != RescueDecision::MissingMethod ||
        empty.RescueOutcome() || empty.View().phase != ExperimentPhase::Rescue) return false;
    // Player death does not fabricate child death, even without discovery or an attack.
    if (Settle(empty, 2, 2, false, true, true).decision != RescueDecision::Failed ||
        empty.RescueOutcome()->childDied || empty.RescueOutcome()->elapsedRealSeconds) return false;
    for (bool sword : {false, true}) {
        auto s = ReadySession(5, sword);
        if (!sword) s.NotifyThreatDiscovered({{5, 1, {1, 1}}});
        s.NotifyActionUsed({{5, 4, {3, 3}}, 2, ActionKind::Magic});
        if (Settle(s, 5, 1, true, true, true).decision != RescueDecision::Succeeded ||
            s.RescueOutcome()->method != (sword ? RescueMethod::Mixed : RescueMethod::Magic)) return false;
    }
    return true;
}
}
