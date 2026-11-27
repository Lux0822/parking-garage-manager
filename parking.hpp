#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class EntryResult {
    Accepted,
    AlreadyParked,
    GarageFull,
    InvalidPlate,
    InvalidArrivalTime
};

enum class ExitStatus {
    Completed,
    VehicleNotFound,
    InvalidDepartureTime,
    DepartureBeforeArrival
};

struct ExitResult {
    ExitStatus status;
    int parkedMinutes = 0;
};

struct ParkedVehicle {
    std::string licensePlate;
    int arrivalMinutes;
};

class ParkingGarage {
public:
    EntryResult enter(const std::string& licensePlate, std::string_view arrivalTime);
    ExitResult previewExit(const std::string& licensePlate, std::string_view departureTime) const;
    ExitResult leave(const std::string& licensePlate, std::string_view departureTime);
    std::optional<int> arrivalMinutes(const std::string& licensePlate) const;
    std::vector<ParkedVehicle> listParkedVehicles() const;
    std::size_t availableSpaces() const;

private:
    static constexpr std::size_t capacity = 10;
    std::unordered_map<std::string, int> parkedVehicles;
};
