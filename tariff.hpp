#pragma once

#include <memory>
#include <string_view>

class ParkingTariff {
public:
    virtual ~ParkingTariff() = default;
    virtual int feeInHuf(int parkedMinutes) const = 0;
};

class StandardTariff final : public ParkingTariff {
public:
    int feeInHuf(int parkedMinutes) const override;
};

class CappedTariff final : public ParkingTariff {
public:
    explicit CappedTariff(std::unique_ptr<ParkingTariff> baseTariff);
    int feeInHuf(int parkedMinutes) const override;

private:
    std::unique_ptr<ParkingTariff> baseTariff;
};

class GracePeriodTariff final : public ParkingTariff {
public:
    explicit GracePeriodTariff(std::unique_ptr<ParkingTariff> baseTariff);
    int feeInHuf(int parkedMinutes) const override;

private:
    std::unique_ptr<ParkingTariff> baseTariff;
};

std::unique_ptr<ParkingTariff> makeTariff(std::string_view name);
