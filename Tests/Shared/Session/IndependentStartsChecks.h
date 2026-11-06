#pragma once
#include "experiment/Session/ExperimentSession.h"
#include <limits>
namespace experiment::checks {
inline bool IndependentStarts() {
    ExperimentSession session{7};
    if (session.NotifyThreatDiscovered({{7, 1, {0, 0}}})) return false;
    if (!session.NotifyWorldReady({7, {0, 0}})) return false;
    if (!session.NotifyActionUsed({{7, 1, {10, 20}}, 1, ActionKind::Magic})) return false;
    if (!session.NotifyAttackAtGoblin({{7, 2, {10, 20}}, 1})) return false;
    auto view = session.View();
    if (view.threatDiscoveredAt || view.magicUsed || !view.firstAttackAt ||
        view.firstAttackAt->realSeconds != 20 || view.worldStartedAt->engineWorldSeconds != 0) return false;
    if (!session.NotifyThreatDiscovered({{7, 3, {30, 60}}})) return false;
    if (session.View().magicUsed) return false; // Earlier projectile never becomes a later use.
    if (!session.NotifyActionUsed({{7, 4, {30, 60}}, 2, ActionKind::Sword})) return false;
    if (!session.NotifyActionUsed({{7, 5, {31, 65}}, 3, ActionKind::Magic})) return false;
    view = session.View();
    if (!view.swordUsed || !view.magicUsed || view.threatDiscoveredAt->realSeconds != 60 ||
        view.firstAttackAt->engineWorldSeconds != 10) return false;
    ExperimentSession observer{8};
    if (!observer.NotifyWorldReady({8, {0, 0}}) ||
        !observer.NotifyThreatDiscovered({{8, 1, {0, 0}}})) return false;
    return observer.View().threatDiscoveredAt.has_value() && !observer.View().firstAttackAt;
}
inline bool FactIdentityAndDelivery() {
    ExperimentSession session{9};
    if (!session.NotifyWorldReady({9, {5, 10}})) return false;
    if (session.NotifyActionUsed({{8, 1, {6, 11}}, 1, ActionKind::Sword}) ||
        session.NotifyThreatDiscovered({{9, 0, {6, 11}}}) ||
        session.NotifyThreatDiscovered({{9, 1, {4, 11}}}) ||
        session.NotifyThreatDiscovered({{9, 1, {6, 9}}}) ||
        session.NotifyThreatDiscovered({{9, 1, {6, std::numeric_limits<double>::quiet_NaN()}}}) ||
        session.NotifyActionUsed({{9, 1, {6, 11}}, 0, ActionKind::Sword}) ||
        session.NotifyActionUsed({{9, 1, {6, 11}}, 1, static_cast<ActionKind>(99)})) return false;
    if (!session.NotifyActionUsed({{9, 4, {8, 14}}, 1, ActionKind::Sword})) return false;
    if (!session.NotifyAttackAtGoblin({{9, 5, {8, 14}}, 1})) return false;
    if (session.NotifyActionUsed({{9, 4, {8, 14}}, 1, ActionKind::Sword}) ||
        session.NotifyActionUsed({{9, 6, {9, 15}}, 1, ActionKind::Magic}) ||
        session.NotifyThreatDiscovered({{9, 5, {8, 14}}}) ||
        session.NotifyAttackAtGoblin({{9, 6, {9, 15}}, 1})) return false;
    // Valid older independent facts survive newer deliveries; original occurrence order wins.
    if (!session.NotifyThreatDiscovered({{9, 3, {7, 13}}}) || !session.View().swordUsed) return false;
    if (!session.NotifyActionUsed({{9, 1, {6, 11}}, 2, ActionKind::Magic}) ||
        session.View().magicUsed) return false;
    if (!session.NotifyAttackAtGoblin({{9, 2, {6, 11}}, 2}) ||
        session.View().firstAttackAt->realSeconds != 11) return false;
    if (session.NotifyThreatDiscovered({{9, 3, {7, 13}}})) return false;
    ExperimentSession next{10};
    if (!next.NotifyWorldReady({10, {0, 0}})) return false;
    return !next.NotifyThreatDiscovered({{9, 3, {7, 13}}}) &&
        !next.NotifyActionUsed({{9, 4, {8, 14}}, 1, ActionKind::Sword}) &&
        !next.NotifyAttackAtGoblin({{9, 5, {8, 14}}, 1}) &&
        !next.View().threatDiscoveredAt && !next.View().firstAttackAt &&
        !next.View().swordUsed && !next.View().magicUsed;
}
}
