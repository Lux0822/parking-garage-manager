cmake_minimum_required(VERSION 3.16)
project(ParkingGarageManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(parking_logic parking.cpp)
add_executable(parking-garage-manager main.cpp)
target_link_libraries(parking-garage-manager PRIVATE parking_logic)

include(CTest)
if(BUILD_TESTING)
    add_executable(parking_tests parking_tests.cpp)
    target_link_libraries(parking_tests PRIVATE parking_logic)
    add_test(NAME vehicle_entry COMMAND parking_tests)
endif()
