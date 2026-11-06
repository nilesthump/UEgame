#include "CoreCaseCatalog.h"
#include <iostream>
#include <string_view>
int main(int argc, char** argv) {
    if (argc != 2) return 2;
    for (const auto& test : experiment::checks::Catalog) {
        if (test.id != std::string_view{argv[1]}) continue;
        const bool passed = test.run();
        std::cout << test.id << (passed ? "=Passed\n" : "=Failed\n");
        return passed ? 0 : 1;
    }
    return 2;
}
