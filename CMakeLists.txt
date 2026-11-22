cmake_minimum_required(VERSION 3.16)
project(ParkingGarageManager LANGUAGES CXX)

add_executable(parking-garage-manager main.cpp)
set_target_properties(parking-garage-manager PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
