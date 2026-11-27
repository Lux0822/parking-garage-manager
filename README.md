# Parking Garage Manager

A C++ command-line application for tracking vehicle entry, exit, and parking fees. The program keeps parking records in memory for the current session.

## Entry and exit

- The garage has 10 spaces. Entry is rejected when the garage is full or the vehicle's license plate is already registered.
- For each successful entry, the garage records the arrival time entered in 24-hour `HH:MM` format.
- On exit, enter the license plate, a departure time on the same day, and the desired tariff. The program frees the space and displays the parking duration and fee.

## Tariffs

- **Standard** charges 400 HUF per started hour.
- **Capped** applies the standard rate with a maximum of 1,200 HUF per stay.

Both tariffs use the same `ParkingTariff` interface, so the exit flow can apply the selected strategy without duplicating fee rules.

## Garage status

Choose `3` to see the available spaces and the parked vehicles. The list shows each license plate and arrival time in license plate order.

## Build, run, and test

Use CMake 3.16 or newer and a C++17 compiler. Run the commands from the repository root.

### Windows with Visual Studio Build Tools

Open a Visual Studio Developer Command Prompt with the C++ workload installed.

```bat
cmake -S . -B build
cmake --build build --config Debug
build\Debug\parking-garage-manager.exe
cmake -E chdir build ctest -C Debug --output-on-failure
```

### Linux

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/parking-garage-manager
cmake -E chdir build ctest --output-on-failure
```

GitHub Actions builds the project and runs both CTest suites on Ubuntu 24.04 on pushes and pull requests.

## Example session

1. Choose `1` and enter `ABC-123` at `09:00`.
2. Choose `3` to see the vehicle and the nine available spaces.
3. Choose `2`, enter `ABC-123` and `13:00`, then choose the capped tariff. The fee is `1200 HUF` for `240` minutes.
4. Choose `3` again to confirm that the garage has ten available spaces.
