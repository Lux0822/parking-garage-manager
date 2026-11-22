#include "tariff.hpp"

#include <algorithm>

int StandardTariff::feeInHuf(int parkedMinutes) const {
    constexpr int minutesPerHour = 60;
    constexpr int hourlyRateInHuf = 400;
    const int startedHours = (parkedMinutes + minutesPerHour - 1) / minutesPerHour;
    return startedHours * hourlyRateInHuf;
}

int CappedTariff::feeInHuf(int parkedMinutes) const {
    constexpr int maximumFeeInHuf = 1200;
    return std::min(StandardTariff{}.feeInHuf(parkedMinutes), maximumFeeInHuf);
}
