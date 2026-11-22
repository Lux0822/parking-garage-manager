#include "parking.hpp"

#include <cctype>

EntryResult ParkingGarage::enter(const std::string& licensePlate) {
    const char* whitespace = " \t\r\n\f\v";
    const auto firstCharacter = licensePlate.find_first_not_of(whitespace);
    if (firstCharacter == std::string::npos) {
        return EntryResult::InvalidPlate;
    }

    const auto lastCharacter = licensePlate.find_last_not_of(whitespace);
    auto normalizedPlate = licensePlate.substr(firstCharacter, lastCharacter - firstCharacter + 1);
    for (char& character : normalizedPlate) {
        character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
    }

    if (parkedVehicles.count(normalizedPlate) != 0) {
        return EntryResult::AlreadyParked;
    }
    if (availableSpaces() == 0) {
        return EntryResult::GarageFull;
    }

    parkedVehicles.insert(normalizedPlate);
    return EntryResult::Accepted;
}

std::size_t ParkingGarage::availableSpaces() const {
    return capacity - parkedVehicles.size();
}
