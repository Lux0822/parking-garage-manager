#include "parking.hpp"

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

void registerExit(ParkingGarage& garage, const std::string& licensePlate, const std::string& time) {
    const auto result = garage.leave(licensePlate, time);
    switch (result.status) {
        case ExitStatus::Completed:
            std::cout << "Parking duration: " << result.parkedMinutes << " minutes.\n"
                      << "Available spaces: " << garage.availableSpaces() << '\n';
            break;
        case ExitStatus::VehicleNotFound:
            std::cout << "This vehicle is not parked.\n";
            break;
        case ExitStatus::InvalidDepartureTime:
            std::cout << "Enter a valid departure time between 00:00 and 23:59.\n";
            break;
        case ExitStatus::DepartureBeforeArrival:
            std::cout << "Departure must be at or after arrival on the same day.\n";
            break;
    }
}

}

int main() {
    ParkingGarage garage;
    std::cout << "Parking Garage Manager\n";

    std::string choice;
    while (std::cout << "Choose [1] Entry, [2] Exit, [Enter] Quit: " &&
           std::getline(std::cin, choice) && !choice.empty()) {
        if (choice != "1" && choice != "2") {
            std::cout << "Choose 1 or 2.\n";
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
