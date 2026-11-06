#pragma once
#include "Session/WorldReadyChecks.h"
#include "Session/IndependentStartsChecks.h"
#include <array>
#include <string_view>
namespace experiment::checks {
struct CoreCase { std::string_view id; bool (*run)(); };
inline constexpr std::array<CoreCase, 3> Catalog{{
    {"B01.Session.WorldReady", &WorldReadyBoundary},
    {"B01.Session.IndependentStarts", &IndependentStarts},
    {"B01.Session.FactIdentityAndDelivery", &FactIdentityAndDelivery}
}};
}
