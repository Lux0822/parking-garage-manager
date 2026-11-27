#include "tariff.hpp"

#include <catch2/catch_test_macros.hpp>

#include <memory>

TEST_CASE("The standard tariff charges for each started hour", "[tariff]") {
    const StandardTariff tariff;
    CHECK(tariff.feeInHuf(0) == 0);
    CHECK(tariff.feeInHuf(1) == 400);
    CHECK(tariff.feeInHuf(60) == 400);
    CHECK(tariff.feeInHuf(61) == 800);
    CHECK(tariff.feeInHuf(240) == 1600);
}

TEST_CASE("The capped tariff limits the fee to 1200 HUF", "[tariff]") {
    const CappedTariff tariff(std::make_unique<StandardTariff>());
    CHECK(tariff.feeInHuf(0) == 0);
    CHECK(tariff.feeInHuf(60) == 400);
    CHECK(tariff.feeInHuf(61) == 800);
    CHECK(tariff.feeInHuf(180) == 1200);
    CHECK(tariff.feeInHuf(181) == 1200);
    CHECK(tariff.feeInHuf(240) == 1200);
}

TEST_CASE("The grace period is deducted before rounding up billable hours", "[tariff]") {
    const GracePeriodTariff tariff(std::make_unique<StandardTariff>());
    CHECK(tariff.feeInHuf(0) == 0);
    CHECK(tariff.feeInHuf(15) == 0);
    CHECK(tariff.feeInHuf(16) == 400);
    CHECK(tariff.feeInHuf(75) == 400);
    CHECK(tariff.feeInHuf(76) == 800);
}

TEST_CASE("The capped tariff can wrap a grace-period tariff", "[tariff]") {
    const CappedTariff tariff(std::make_unique<GracePeriodTariff>(std::make_unique<StandardTariff>()));
    CHECK(tariff.feeInHuf(15) == 0);
    CHECK(tariff.feeInHuf(75) == 400);
    CHECK(tariff.feeInHuf(240) == 1200);
}

TEST_CASE("The tariff factory selects a strategy by name", "[tariff]") {
    const auto standard = makeTariff("standard");
    const auto capped = makeTariff("capped");
    const auto grace = makeTariff("grace");
    REQUIRE(standard);
    REQUIRE(capped);
    REQUIRE(grace);
    CHECK(standard->feeInHuf(240) == 1600);
    CHECK(capped->feeInHuf(240) == 1200);
    CHECK(grace->feeInHuf(15) == 0);
    CHECK_FALSE(makeTariff("unknown"));
}
