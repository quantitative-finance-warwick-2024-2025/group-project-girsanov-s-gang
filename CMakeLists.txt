# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.14)

# Project's name
project(IB9JHO_Group_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Main application executable
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

# Generate convergence output executable
add_executable(generate_convergence_output 
    ${CMAKE_SOURCE_DIR}/src/generate_convergence_output.cpp 
    ${CMAKE_SOURCE_DIR}/src/capture_output.cpp
)
target_include_directories(generate_convergence_output PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/convergence_output)

# Create a custom target to run generate_convergence_output and produce the CSV
add_custom_target(run_generate_convergence_output ALL
  COMMAND $<TARGET_FILE:generate_convergence_output> --output ${CMAKE_BINARY_DIR}/convergence_output/output.csv
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Use the build directory as working directory
  COMMENT "Running generate_convergence_output to create output CSV..."
)

# Add test files directory (if you still need to run tests separately)
add_subdirectory(tests)

# Add library
add_library(ib9jho_library src/Option.cpp src/AsianOption.cpp src/PricingEngine.cpp)
target_include_directories(ib9jho_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
