#include "parking.hpp"
#include "tariff.hpp"

#include <iomanip>
#include <iostream>
#include <string>

namespace {

void registerEntry(ParkingGarage& garage, const std::string& licensePlate, const std::string& time) {
    switch (garage.enter(licensePlate, time)) {
        case EntryResult::Accepted:
            std::cout << "Vehicle admitted. Available spaces: " << garage.availableSpaces() << '\n';
            break;
        case EntryResult::AlreadyParked:
            std::cout << "This vehicle is already parked.\n";
            break;
        case EntryResult::GarageFull:
            std::cout << "The garage is full.\n";
            break;
        case EntryResult::InvalidPlate:
            std::cout << "The license plate must contain a non-whitespace character.\n";
            break;
        case EntryResult::InvalidArrivalTime:
            std::cout << "Enter a valid arrival time between 00:00 and 23:59.\n";
            break;
    }
}

const ParkingTariff* selectTariff() {
    static const StandardTariff standardTariff;
    static const CappedTariff cappedTariff;

    std::string choice;
    while (std::cout << "Choose [1] Standard, [2] Capped, [Enter] Cancel exit: " &&
           std::getline(std::cin, choice) && !choice.empty()) {
        if (choice == "1") {
            return &standardTariff;
        }
        if (choice == "2") {
            return &cappedTariff;
        }
        std::cout << "Choose 1 or 2.\n";
    }
    return nullptr;
}

void registerExit(ParkingGarage& garage, const std::string& licensePlate, const std::string& time) {
    const auto preview = garage.previewExit(licensePlate, time);
    switch (preview.status) {
        case ExitStatus::Completed:
            break;
        case ExitStatus::VehicleNotFound:
            std::cout << "This vehicle is not parked.\n";
            return;
        case ExitStatus::InvalidDepartureTime:
            std::cout << "Enter a valid departure time between 00:00 and 23:59.\n";
            return;
        case ExitStatus::DepartureBeforeArrival:
            std::cout << "Departure must be at or after arrival on the same day.\n";
            return;
    }

    const auto* tariff = selectTariff();
    if (!tariff) {
        return;
    }

    const int fee = tariff->feeInHuf(preview.parkedMinutes);
    garage.leave(licensePlate, time);
    std::cout << "Parking duration: " << preview.parkedMinutes << " minutes.\n"
              << "Parking fee: " << fee << " HUF.\n"
              << "Available spaces: " << garage.availableSpaces() << '\n';
}

void showStatus(const ParkingGarage& garage) {
    std::cout << "Available spaces: " << garage.availableSpaces() << '\n';
    const auto vehicles = garage.listParkedVehicles();
    if (vehicles.empty()) {
        std::cout << "No vehicles parked.\n";
        return;
    }

    std::cout << "Parked vehicles (license plate, arrival time):\n";
    const char previousFill = std::cout.fill('0');
    for (const auto& vehicle : vehicles) {
        std::cout << vehicle.licensePlate << "  "
                  << std::setw(2) << vehicle.arrivalMinutes / 60 << ':'
                  << std::setw(2) << vehicle.arrivalMinutes % 60 << '\n';
    }
    std::cout.fill(previousFill);
}

}

int main() {
    ParkingGarage garage;
    std::cout << "Parking Garage Manager\n";

    std::string choice;
    while (std::cout << "Choose [1] Entry, [2] Exit, [3] Status, [Enter] Quit: " &&
           std::getline(std::cin, choice) && !choice.empty()) {
        if (choice == "3") {
            showStatus(garage);
            continue;
        }
        if (choice != "1" && choice != "2") {
            std::cout << "Choose 1, 2, or 3.\n";
            continue;
        }

        std::string licensePlate;
        std::cout << "Enter a license plate: ";
        if (!std::getline(std::cin, licensePlate)) {
            break;
        }

        const bool isEntry = choice == "1";
        std::string time;
        std::cout << (isEntry ? "Enter the arrival time (HH:MM): " : "Enter the departure time (HH:MM): ");
        if (!std::getline(std::cin, time)) {
            break;
        }

        if (isEntry) {
            registerEntry(garage, licensePlate, time);
        } else {
            registerExit(garage, licensePlate, time);
        }
    }

    return 0;
}
