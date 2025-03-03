# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.14)

# Project's name
project(IB9JHO_Group_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Main application executable (if needed)
add_executable(Girsanov_gang_proj
    src/main.cpp
    src/AsianOption.cpp
    src/PricingEngine.cpp
    src/Option.cpp
    src/BarrierOption.cpp
    src/OptionPricerBarrier.cpp
    src/AnalysisWriter.cpp
)

# Expose project root directory to source code so it can find the files
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Create a library with common sources for option pricing and barrier option logic.
add_library(ib9jho_library 
    src/Option.cpp 
    src/AsianOption.cpp 
    src/PricingEngine.cpp
    src/BarrierOption.cpp
    src/OptionPricerBarrier.cpp
    src/AnalysisWriter.cpp
)
target_include_directories(ib9jho_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Option Pricing Convergence Target
# This executable prints CSV data to stdout.
# Build only the unique source file (option_pricing_convergence.cpp).
add_executable(option_pricing_convergence 
    ${CMAKE_SOURCE_DIR}/src/option_pricing_convergence.cpp
)
target_include_directories(option_pricing_convergence PUBLIC ${CMAKE_SOURCE_DIR}/src)
# Link with the common library so that all symbols (from Option, BarrierOption, etc.) are available.
target_link_libraries(option_pricing_convergence PUBLIC ib9jho_library)

# Generate Convergence Output Target
# This executable calls option_pricing_convergence and redirects its output.
add_executable(generate_convergence_output 
    ${CMAKE_SOURCE_DIR}/src/generate_convergence_output.cpp 
    ${CMAKE_SOURCE_DIR}/src/capture_output.cpp
)
target_include_directories(generate_convergence_output PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/convergence_output)

# Ensure that option_pricing_convergence is built before generate_convergence_output runs.
add_dependencies(generate_convergence_output option_pricing_convergence)

# Create a custom target to run generate_convergence_output and produce the CSV.
add_custom_target(run_generate_convergence_output ALL
  COMMAND $<TARGET_FILE:generate_convergence_output> --output "${CMAKE_SOURCE_DIR}/src/output.csv"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Running generate_convergence_output to create output CSV in src folder..."
)

# Add test files directory (if needed)
add_subdirectory(tests)