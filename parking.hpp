#pragma once

#include <cstddef>
#include <string>
#include <unordered_set>

enum class EntryResult {
    Accepted,
    AlreadyParked,
    GarageFull,
    InvalidPlate
};

class ParkingGarage {
public:
    EntryResult enter(const std::string& licensePlate);
    std::size_t availableSpaces() const;

private:
    static constexpr std::size_t capacity = 10;
    std::unordered_set<std::string> parkedVehicles;
};
