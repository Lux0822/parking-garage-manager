#include "tariff.hpp"

#include <iostream>

int main() {
    const StandardTariff standard;
    const CappedTariff capped;

    if (standard.feeInHuf(0) != 0 || capped.feeInHuf(0) != 0) {
        std::cerr << "Both tariffs should charge zero for zero minutes.\n";
        return 1;
    }
    if (standard.feeInHuf(1) != 400 || standard.feeInHuf(60) != 400 ||
        standard.feeInHuf(61) != 800 || standard.feeInHuf(240) != 1600) {
        std::cerr << "The standard tariff should charge 400 HUF per started hour.\n";
        return 1;
    }
    if (capped.feeInHuf(60) != 400 || capped.feeInHuf(61) != 800 ||
        capped.feeInHuf(180) != 1200 || capped.feeInHuf(181) != 1200 ||
        capped.feeInHuf(240) != 1200) {
        std::cerr << "The capped tariff should apply the standard rate up to 1,200 HUF.\n";
        return 1;
    }
}
