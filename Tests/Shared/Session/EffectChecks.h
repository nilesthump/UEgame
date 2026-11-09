#pragma once
#include "SettlementChecks.h"
namespace experiment::checks {
inline bool EffectEligibility() {
    for (bool success : {false, true}) {
        auto session = ReadySession(41);
        const auto snapshot = session.NotifyAftermathObserved({{41, 4, {3, 3}}}).commands.front();
        auto forged = snapshot;
        forged.kind = EffectKind::StopRun;
        if (session.TryConsumeEffect(forged)) return false;
        const auto effects = Settle(session, 5, 1, true, success, true);
        if (effects.commands.size() != 2 || session.TryConsumeEffect(snapshot)) return false;
        // Cleanup remains eligible in Ended, but progression cannot overtake it.
        if (session.TryConsumeEffect(effects.commands[1]) ||
            !session.TryConsumeEffect(effects.commands[0]) ||
            session.TryConsumeEffect(effects.commands[0]) ||
            !session.TryConsumeEffect(effects.commands[1]) ||
            session.TryConsumeEffect(effects.commands[1])) return false;
        auto next = ReadySession(42);
        next.NotifyAftermathObserved({{42, 4, {3, 3}}});
        if (next.TryConsumeEffect(snapshot)) return false;
    }
    auto session = ReadySession(43);
    const auto old = session.NotifyAftermathObserved({{43, 4, {3, 3}}}).commands.front();
    session.InvalidatePendingEffects();
    const auto current = session.NotifyAftermathObserved({{43, 5, {4, 4}}}).commands.front();
    if (old.requestId == current.requestId || session.TryConsumeEffect(old)) return false;
    auto wrong = current;
    wrong.phase = ExperimentPhase::Ended;
    if (session.TryConsumeEffect(wrong)) return false;
    wrong = current;
    ++wrong.requestId;
    if (session.TryConsumeEffect(wrong) || !session.TryConsumeEffect(current) ||
        session.TryConsumeEffect(current)) return false;
    const auto ended = Settle(session, 6, 1, false, true, false);
    session.InvalidatePendingEffects();
    return !session.TryConsumeEffect(ended.commands[0]) &&
        !session.TryConsumeEffect(ended.commands[1]);
}
}
