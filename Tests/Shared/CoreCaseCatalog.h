#pragma once
#include "Session/WorldReadyChecks.h"
#include <array>
#include <string_view>
namespace experiment::checks {
struct CoreCase { std::string_view id; bool (*run)(); };
inline constexpr std::array<CoreCase, 1> Catalog{{
    {"B01.Session.WorldReady", &WorldReadyBoundary}
}};
}
