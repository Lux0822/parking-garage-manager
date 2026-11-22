#include "parking.hpp"

#include <iostream>
#include <string>

int main() {
    ParkingGarage garage;
    std::cout << "Parking Garage Manager\n";

    std::string licensePlate;
    while (std::cout << "Enter a license plate (Enter to quit): " &&
           std::getline(std::cin, licensePlate) && !licensePlate.empty()) {
        std::string arrivalTime;
        std::cout << "Enter the arrival time (HH:MM): ";
        if (!std::getline(std::cin, arrivalTime)) {
            break;
        }

        switch (garage.enter(licensePlate, arrivalTime)) {
            case EntryResult::Accepted:
                std::cout << "Vehicle admitted. Available spaces: "
                          << garage.availableSpaces() << '\n';
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

    return 0;
}
