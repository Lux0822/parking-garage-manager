# Parking Garage Manager

A C++ command-line application for tracking vehicle entry, exit, and parking fees.

## Entry and exit

- The garage has 10 spaces. Entry is rejected when the garage is full or the vehicle's license plate is already registered.
- For each successful entry, the garage records the arrival time entered in 24-hour `HH:MM` format.
- On exit, enter the license plate, a departure time on the same day, and the desired tariff. The program displays the parking duration and fee, then frees the space.

## Tariffs

- **Standard** charges 400 HUF per started hour.
- **Capped** applies the standard rate with a maximum of 1,200 HUF per stay.

Both tariffs use the same `ParkingTariff` interface, so the exit flow can apply the selected strategy without duplicating fee rules.

## Garage status

Choose `3` to see the available spaces and the parked vehicles. The list shows each license plate and arrival time in license plate order.

## Build

Use CMake 3.16 or newer and a C++17 compiler.

```sh
cmake -S . -B build
cmake --build build
```

## Test

From the `build` directory, run `ctest -C Debug --output-on-failure` after building.
