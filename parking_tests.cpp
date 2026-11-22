#include "parking.hpp"

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

int main() {
    ParkingGarage garage;

    if (garage.enter("ABC-123", "14:30") != EntryResult::Accepted || garage.availableSpaces() != 9) {
        std::cerr << "The first vehicle should be admitted and leave nine spaces.\n";
        return 1;
    }
    if (garage.enter(" abc-123 ", "15:00") != EntryResult::AlreadyParked || garage.availableSpaces() != 9) {
        std::cerr << "A duplicate plate should be rejected without using a space.\n";
        return 1;
    }
    if (garage.arrivalMinutes(" abc-123 ") != 870) {
        std::cerr << "The original arrival time of 14:30 should be stored as 870 minutes.\n";
        return 1;
    }
    if (garage.enter("   ", "14:30") != EntryResult::InvalidPlate || garage.availableSpaces() != 9) {
        std::cerr << "A blank plate should be rejected without using a space.\n";
        return 1;
    }
    for (const std::string_view invalidTime : {"25:70", "24:00", "23:60", "9:15", "12.30", "1a:30"}) {
        if (garage.enter("XYZ-123", invalidTime) != EntryResult::InvalidArrivalTime ||
            garage.availableSpaces() != 9 || garage.arrivalMinutes("XYZ-123")) {
            std::cerr << "An invalid arrival time should be rejected without using a space.\n";
            return 1;
        }
    }
    for (int number = 1; number <= 9; ++number) {
        if (garage.enter("CAR-" + std::to_string(number), "00:00") != EntryResult::Accepted) {
            std::cerr << "Vehicles should be admitted until all ten spaces are occupied.\n";
            return 1;
        }
    }
    if (garage.enter("CAR-10", "23:59") != EntryResult::GarageFull || garage.availableSpaces() != 0) {
        std::cerr << "The eleventh vehicle should be rejected and no spaces should remain.\n";
        return 1;
    }

    return 0;
}
