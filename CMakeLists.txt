# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.14)

# Project's name
project(IB9JHO_Group_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
add_executable(Girsanov_gang_proj
    src/main.cpp
    src/AsianOption.cpp
    src/PricingEngine.cpp
    src/Option.cpp
    src/BarrierOption.cpp
    src/OptionPricerBarrier.cpp
    src/AnalysisWriter.cpp
)

# Add test files directory
add_subdirectory(tests)

# Add library
add_library(ib9jho_library src/Option.cpp src/AsianOption.cpp src/PricingEngine.cpp)

# Include directories for header files
target_include_directories(ib9jho_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
