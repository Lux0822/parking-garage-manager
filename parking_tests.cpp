#include "parking.hpp"

#include <iostream>
#include <string>

int main() {
    ParkingGarage garage;

    if (garage.enter("ABC-123") != EntryResult::Accepted || garage.availableSpaces() != 9) {
        std::cerr << "The first vehicle should be admitted and leave nine spaces.\n";
        return 1;
    }
    if (garage.enter(" abc-123 ") != EntryResult::AlreadyParked || garage.availableSpaces() != 9) {
        std::cerr << "A duplicate plate should be rejected without using a space.\n";
        return 1;
    }
    if (garage.enter("   ") != EntryResult::InvalidPlate || garage.availableSpaces() != 9) {
        std::cerr << "A blank plate should be rejected without using a space.\n";
        return 1;
    }
    for (int number = 1; number <= 9; ++number) {
        if (garage.enter("CAR-" + std::to_string(number)) != EntryResult::Accepted) {
            std::cerr << "Vehicles should be admitted until all ten spaces are occupied.\n";
            return 1;
        }
    }
    if (garage.enter("CAR-10") != EntryResult::GarageFull || garage.availableSpaces() != 0) {
        std::cerr << "The eleventh vehicle should be rejected and no spaces should remain.\n";
        return 1;
    }

    return 0;
}
