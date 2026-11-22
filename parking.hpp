#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

enum class EntryResult {
    Accepted,
    AlreadyParked,
    GarageFull,
    InvalidPlate,
    InvalidArrivalTime
};

class ParkingGarage {
public:
    EntryResult enter(const std::string& licensePlate, std::string_view arrivalTime);
    std::optional<int> arrivalMinutes(const std::string& licensePlate) const;
    std::size_t availableSpaces() const;

private:
    static constexpr std::size_t capacity = 10;
    std::unordered_map<std::string, int> parkedVehicles;
};
