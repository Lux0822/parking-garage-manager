#pragma once

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
    int feeInHuf(int parkedMinutes) const override;
};
