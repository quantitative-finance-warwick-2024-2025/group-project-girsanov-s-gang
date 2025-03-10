cmake_minimum_required(VERSION 3.15)
project(MyProjectTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/convergence_output)

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
# Barrier pricing tests
################################
# Barrier pricing tests
add_executable(naive_pricing
    naive_pricing.cpp
    ../src/Option.cpp
    ../src/BarrierOption.cpp
    ../src/OptionPricerBarrier.cpp
)
target_include_directories(naive_pricing PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME naive_pricing COMMAND naive_pricing)


add_executable(antithetic_pricing
    antithetic_pricing.cpp
    ../src/Option.cpp
    ../src/BarrierOption.cpp
    ../src/OptionPricerBarrier.cpp
)
target_include_directories(antithetic_pricing PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME antithetic_pricing COMMAND antithetic_pricing)

add_executable(control_variates_pricing
    control_variates_pricing.cpp
    ../src/Option.cpp
    ../src/BarrierOption.cpp
    ../src/OptionPricerBarrier.cpp
)
target_include_directories(control_variates_pricing PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME control_variates_pricing COMMAND control_variates_pricing)

add_executable(importance_sampling_pricing
    importance_sampling_pricing.cpp
    ../src/Option.cpp
    ../src/BarrierOption.cpp
    ../src/OptionPricerBarrier.cpp
)
target_include_directories(importance_sampling_pricing PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/catch2
)
add_test(NAME importance_sampling_pricing COMMAND importance_sampling_pricing)