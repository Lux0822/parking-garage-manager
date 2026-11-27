#include "parking.hpp"

#include <catch2/catch_test_macros.hpp>

#include <initializer_list>
#include <string>

TEST_CASE("An empty garage has ten available spaces", "[entry][status]") {
    const ParkingGarage garage;
    CHECK(garage.availableSpaces() == 10);
    CHECK(garage.listParkedVehicles().empty());
}

TEST_CASE("Entry records the original time and rejects duplicate plates", "[entry]") {
    ParkingGarage garage;
    REQUIRE(garage.enter("ABC-123", "14:30") == EntryResult::Accepted);
    CHECK(garage.enter(" abc-123 ", "15:00") == EntryResult::AlreadyParked);
    CHECK(garage.arrivalMinutes(" abc-123 ") == 870);
    CHECK(garage.availableSpaces() == 9);
}

TEST_CASE("Entry rejects a blank plate", "[entry]") {
    ParkingGarage garage;
    CHECK(garage.enter("   ", "14:30") == EntryResult::InvalidPlate);
    CHECK(garage.availableSpaces() == 10);
}

TEST_CASE("Entry rejects malformed and out-of-range times", "[entry]") {
    ParkingGarage garage;
    for (const char* time : {"25:70", "24:00", "23:60", "9:15", "12.30", "1a:30"}) {
        CAPTURE(time);
        CHECK(garage.enter("ABC-123", time) == EntryResult::InvalidArrivalTime);
    }
    CHECK(garage.availableSpaces() == 10);
    CHECK_FALSE(garage.arrivalMinutes("ABC-123"));
}

TEST_CASE("A full garage admits another vehicle after a departure", "[entry][exit]") {
    ParkingGarage garage;
    for (int vehicle = 1; vehicle <= 10; ++vehicle) {
        REQUIRE(garage.enter("CAR-" + std::to_string(vehicle), "00:00") == EntryResult::Accepted);
    }
    CHECK(garage.enter("CAR-11", "00:00") == EntryResult::GarageFull);
    CHECK(garage.availableSpaces() == 0);
    REQUIRE(garage.leave("CAR-1", "01:00").status == ExitStatus::Completed);
    CHECK(garage.enter("CAR-11", "01:00") == EntryResult::Accepted);
}

TEST_CASE("A departure preview reports the duration and preserves the space", "[exit]") {
    ParkingGarage garage;
    REQUIRE(garage.enter("ABC-123", "14:30") == EntryResult::Accepted);
    const auto preview = garage.previewExit(" abc-123 ", "15:31");
    REQUIRE(preview.status == ExitStatus::Completed);
    CHECK(preview.parkedMinutes == 61);
    CHECK(garage.availableSpaces() == 9);
    CHECK(garage.arrivalMinutes("ABC-123") == 870);
}

TEST_CASE("Invalid departures preserve the parking record", "[exit]") {
    ParkingGarage garage;
    REQUIRE(garage.enter("ABC-123", "14:30") == EntryResult::Accepted);
    CHECK(garage.leave("XYZ-123", "15:31").status == ExitStatus::VehicleNotFound);
    CHECK(garage.leave("ABC-123", "15:60").status == ExitStatus::InvalidDepartureTime);
    CHECK(garage.leave("ABC-123", "14:29").status == ExitStatus::DepartureBeforeArrival);
    CHECK(garage.arrivalMinutes("ABC-123") == 870);
    CHECK(garage.availableSpaces() == 9);
}

TEST_CASE("A completed departure removes the record and allows re-entry", "[exit]") {
    ParkingGarage garage;
    REQUIRE(garage.enter("ABC-123", "14:30") == EntryResult::Accepted);
    const auto departure = garage.leave(" abc-123 ", "15:31");
    REQUIRE(departure.status == ExitStatus::Completed);
    CHECK(departure.parkedMinutes == 61);
    CHECK(garage.availableSpaces() == 10);
    CHECK_FALSE(garage.arrivalMinutes("ABC-123"));

    REQUIRE(garage.enter("ABC-123", "15:31") == EntryResult::Accepted);
    const auto immediateDeparture = garage.leave("ABC-123", "15:31");
    REQUIRE(immediateDeparture.status == ExitStatus::Completed);
    CHECK(immediateDeparture.parkedMinutes == 0);
}

TEST_CASE("Status sorts parked vehicles by plate and excludes departed vehicles", "[status]") {
    ParkingGarage garage;
    REQUIRE(garage.enter("ZXY-321", "09:05") == EntryResult::Accepted);
    REQUIRE(garage.enter("ABC-123", "14:30") == EntryResult::Accepted);
    const auto beforeExit = garage.listParkedVehicles();
    REQUIRE(beforeExit.size() == 2);
    CHECK(beforeExit[0].licensePlate == "ABC-123");
    CHECK(beforeExit[0].arrivalMinutes == 870);
    CHECK(beforeExit[1].licensePlate == "ZXY-321");
    CHECK(beforeExit[1].arrivalMinutes == 545);

    REQUIRE(garage.leave("ABC-123", "15:31").status == ExitStatus::Completed);
    const auto afterExit = garage.listParkedVehicles();
    REQUIRE(afterExit.size() == 1);
    CHECK(afterExit.front().licensePlate == "ZXY-321");
}
