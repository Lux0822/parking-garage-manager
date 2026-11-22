# Parking Garage Manager

A C++ command-line application for tracking vehicle entry, exit, and parking fees.

## Planned behavior

- The garage has 10 spaces. Entry is rejected when the garage is full or the vehicle's license plate is already registered.
- For each successful entry, the garage records the arrival time entered in 24-hour `HH:MM` format.
- On exit, the program calculates the fee from the parking duration and selected tariff, then frees the space.
- The status view lists parked vehicles and shows how many spaces are available.

The standard tariff charges 400 HUF per started hour. The capped tariff uses the same hourly rate with a maximum of 1,200 HUF per stay.

## Build

Use CMake 3.16 or newer and a C++17 compiler.

```sh
cmake -S . -B build
cmake --build build
```

## Test

From the `build` directory, run `ctest -C Debug --output-on-failure` after building.
