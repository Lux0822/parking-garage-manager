#include "tariff.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

int StandardTariff::feeInHuf(int parkedMinutes) const {
    constexpr int minutesPerHour = 60;
    constexpr int hourlyRateInHuf = 400;
    const int startedHours = (parkedMinutes + minutesPerHour - 1) / minutesPerHour;
    return startedHours * hourlyRateInHuf;
}

CappedTariff::CappedTariff(std::unique_ptr<ParkingTariff> baseTariff)
    : baseTariff(std::move(baseTariff)) {
    if (!this->baseTariff) {
        throw std::invalid_argument("A capped tariff needs a base tariff.");
    }
}

int CappedTariff::feeInHuf(int parkedMinutes) const {
    constexpr int maximumFeeInHuf = 1200;
    return std::min(baseTariff->feeInHuf(parkedMinutes), maximumFeeInHuf);
}

GracePeriodTariff::GracePeriodTariff(std::unique_ptr<ParkingTariff> baseTariff)
    : baseTariff(std::move(baseTariff)) {
    if (!this->baseTariff) {
        throw std::invalid_argument("A grace-period tariff needs a base tariff.");
    }
}

int GracePeriodTariff::feeInHuf(int parkedMinutes) const {
    constexpr int freeMinutes = 15;
    return baseTariff->feeInHuf(std::max(0, parkedMinutes - freeMinutes));
}

std::unique_ptr<ParkingTariff> makeTariff(std::string_view name) {
    if (name == "standard") {
        return std::make_unique<StandardTariff>();
    }
    if (name == "capped") {
        return std::make_unique<CappedTariff>(std::make_unique<StandardTariff>());
    }
    if (name == "grace") {
        return std::make_unique<GracePeriodTariff>(std::make_unique<StandardTariff>());
    }
    return nullptr;
}
