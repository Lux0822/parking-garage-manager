#include "parking.hpp"

#include <algorithm>
#include <cctype>
#include <initializer_list>

namespace {

std::string normalizeLicensePlate(const std::string& licensePlate) {
    const char* whitespace = " \t\r\n\f\v";
    const auto firstCharacter = licensePlate.find_first_not_of(whitespace);
    if (firstCharacter == std::string::npos) {
        return {};
    }

    const auto lastCharacter = licensePlate.find_last_not_of(whitespace);
    auto normalizedPlate = licensePlate.substr(firstCharacter, lastCharacter - firstCharacter + 1);
    for (char& character : normalizedPlate) {
        character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    }
    return normalizedPlate;
}

std::optional<int> parseTime(std::string_view time) {
    if (time.size() != 5 || time[2] != ':') {
        return std::nullopt;
    }
    for (const char digit : {time[0], time[1], time[3], time[4]}) {
        if (digit < '0' || digit > '9') {
            return std::nullopt;
        }
    }

    const int hour = (time[0] - '0') * 10 + (time[1] - '0');
    const int minute = (time[3] - '0') * 10 + (time[4] - '0');
    if (hour > 23 || minute > 59) {
        return std::nullopt;
    }
    return hour * 60 + minute;
}

}

EntryResult ParkingGarage::enter(const std::string& licensePlate, std::string_view arrivalTime) {
    const auto normalizedPlate = normalizeLicensePlate(licensePlate);
    if (normalizedPlate.empty()) {
        return EntryResult::InvalidPlate;
    }

    if (parkedVehicles.count(normalizedPlate) != 0) {
        return EntryResult::AlreadyParked;
    }
    if (availableSpaces() == 0) {
        return EntryResult::GarageFull;
    }

    const auto minutes = parseTime(arrivalTime);
    if (!minutes) {
        return EntryResult::InvalidArrivalTime;
    }

    parkedVehicles.emplace(normalizedPlate, *minutes);
    return EntryResult::Accepted;
}

ExitResult ParkingGarage::leave(const std::string& licensePlate, std::string_view departureTime) {
    const auto vehicle = parkedVehicles.find(normalizeLicensePlate(licensePlate));
    if (vehicle == parkedVehicles.end()) {
        return {ExitStatus::VehicleNotFound};
    }

    const auto departureMinutes = parseTime(departureTime);
    if (!departureMinutes) {
        return {ExitStatus::InvalidDepartureTime};
    }

    const int parkedMinutes = *departureMinutes - vehicle->second;
    if (parkedMinutes < 0) {
        return {ExitStatus::DepartureBeforeArrival};
    }

    parkedVehicles.erase(vehicle);
    return {ExitStatus::Completed, parkedMinutes};
}

std::optional<int> ParkingGarage::arrivalMinutes(const std::string& licensePlate) const {
    const auto vehicle = parkedVehicles.find(normalizeLicensePlate(licensePlate));
    if (vehicle == parkedVehicles.end()) {
        return std::nullopt;
    }
    return vehicle->second;
}

std::vector<ParkedVehicle> ParkingGarage::listParkedVehicles() const {
    std::vector<ParkedVehicle> vehicles;
    vehicles.reserve(parkedVehicles.size());
    for (const auto& [plate, minutes] : parkedVehicles) {
        vehicles.push_back({plate, minutes});
    }
    std::sort(vehicles.begin(), vehicles.end(), [](const ParkedVehicle& left, const ParkedVehicle& right) {
        return left.licensePlate < right.licensePlate;
    });
    return vehicles;
}

std::size_t ParkingGarage::availableSpaces() const {
    return capacity - parkedVehicles.size();
}
