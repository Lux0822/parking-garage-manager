#include "parking.hpp"

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

int main() {
    ParkingGarage garage;

    if (!garage.listParkedVehicles().empty() || garage.availableSpaces() != 10) {
        std::cerr << "An empty garage should list no vehicles and have ten available spaces.\n";
        return 1;
    }
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

    const auto parkedVehicles = garage.listParkedVehicles();
    if (parkedVehicles.size() != 10 || parkedVehicles[0].licensePlate != "ABC-123" ||
        parkedVehicles[0].arrivalMinutes != 870 || parkedVehicles[1].licensePlate != "CAR-1" ||
        parkedVehicles[1].arrivalMinutes != 0) {
        std::cerr << "The status list should include parked plates in order with their arrival times.\n";
        return 1;
    }

    if (garage.leave("CAR-10", "15:31").status != ExitStatus::VehicleNotFound ||
        garage.availableSpaces() != 0) {
        std::cerr << "An unknown vehicle should not free a space.\n";
        return 1;
    }
    if (garage.leave("ABC-123", "15:60").status != ExitStatus::InvalidDepartureTime ||
        garage.leave("ABC-123", "14:29").status != ExitStatus::DepartureBeforeArrival ||
        garage.availableSpaces() != 0 || garage.arrivalMinutes("ABC-123") != 870) {
        std::cerr << "An invalid departure should leave the parking record intact.\n";
        return 1;
    }
    const auto preview = garage.previewExit(" abc-123 ", "15:31");
    if (preview.status != ExitStatus::Completed || preview.parkedMinutes != 61 ||
        garage.availableSpaces() != 0 || garage.arrivalMinutes("ABC-123") != 870) {
        std::cerr << "Previewing a departure should preserve the parking record.\n";
        return 1;
    }
    const auto departure = garage.leave(" abc-123 ", "15:31");
    if (departure.status != ExitStatus::Completed || departure.parkedMinutes != 61 ||
        garage.availableSpaces() != 1 || garage.arrivalMinutes("ABC-123")) {
        std::cerr << "A successful departure should report 61 minutes and free one space.\n";
        return 1;
    }
    const auto remainingVehicles = garage.listParkedVehicles();
    if (remainingVehicles.size() != 9 || remainingVehicles.front().licensePlate != "CAR-1") {
        std::cerr << "A vehicle should disappear from the status list after leaving.\n";
        return 1;
    }
    if (garage.enter("ABC-123", "15:31") != EntryResult::Accepted) {
        std::cerr << "A vehicle should be able to enter again after leaving.\n";
        return 1;
    }
    const auto immediateDeparture = garage.leave("ABC-123", "15:31");
    if (immediateDeparture.status != ExitStatus::Completed || immediateDeparture.parkedMinutes != 0) {
        std::cerr << "Arrival and departure at the same minute should give a duration of zero.\n";
        return 1;
    }

    return 0;
}
