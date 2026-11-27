cmake_minimum_required(VERSION 3.24)
project(ParkingGarageManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PARKING_ENABLE_SANITIZERS "Enable address and undefined behavior checks on Linux" OFF)

add_library(parking_build_options INTERFACE)
if(MSVC)
    target_compile_options(parking_build_options INTERFACE /W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
    target_compile_options(parking_build_options INTERFACE -Wall -Wextra -Wpedantic)
endif()

if(PARKING_ENABLE_SANITIZERS)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
        message(FATAL_ERROR "Sanitizers require GCC or Clang on Linux.")
    endif()
    target_compile_options(parking_build_options INTERFACE
        -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined)
    target_link_options(parking_build_options INTERFACE -fsanitize=address,undefined)
endif()

add_library(parking_logic parking.cpp tariff.cpp)
target_link_libraries(parking_logic PUBLIC parking_build_options)
add_executable(parking-garage-manager main.cpp)
target_link_libraries(parking-garage-manager PRIVATE parking_logic)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(Catch2
        URL https://codeload.github.com/catchorg/Catch2/tar.gz/refs/tags/v3.8.1
        URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
        DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(parking_tests parking_tests.cpp tariff_tests.cpp)
    target_link_libraries(parking_tests PRIVATE parking_logic Catch2::Catch2WithMain)

    include("${catch2_SOURCE_DIR}/extras/Catch.cmake")
    catch_discover_tests(parking_tests DISCOVERY_MODE PRE_TEST)
endif()
