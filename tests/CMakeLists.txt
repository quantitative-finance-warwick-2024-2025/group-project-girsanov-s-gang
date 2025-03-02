cmake_minimum_required(VERSION 3.15)
project(MyProjectTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing via CTest
enable_testing()

################################
# Option tests
################################
add_executable(option_tests
    test_option.cpp
    ../src/Option.cpp
)
target_include_directories(option_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME option_tests COMMAND option_tests)

################################
# Testing Barrier Option class
################################
add_executable(barrier_tests
    test_barrier_option.cpp
    ../src/BarrierOption.cpp
    ../src/Option.cpp
)
target_include_directories(barrier_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME barrier_tests COMMAND barrier_tests)

################################
# Asian pricing tests
################################
add_executable(asian_pricing_tests
    test_asian_pricing_engine.cpp
    ../src/PricingEngine.cpp
    ../src/Option.cpp
    ../src/AsianOption.cpp
    ../src/BarrierOption.cpp
)
target_include_directories(asian_pricing_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME asian_pricing_tests COMMAND asian_pricing_tests)

################################
# Barrier pricing tests
################################
# Barrier pricing tests
add_executable(barrier_pricing_tests
    barrier_pricing_tests.cpp
    ../src/Option.cpp
    ../src/BarrierOption.cpp
    ../src/OptionPricerBarrier.cpp
)
target_include_directories(barrier_pricing_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME barrier_pricing_tests COMMAND barrier_pricing_tests)