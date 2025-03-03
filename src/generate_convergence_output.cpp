#include <filesystem>
#include <iostream>
#include "capture_output.hpp"

int main()
{
    // Assume we are running from the build directory.
    const std::filesystem::path root = std::filesystem::current_path();

    // On Windows, the executable will have a ".exe" extension.
    #if defined(_WIN32) || defined(_WIN64)
      const std::filesystem::path exe_name = "option_pricing_convergence.exe";
    #else
      const std::filesystem::path exe_name = "option_pricing_convergence";
    #endif

    // The convergence executable is expected to be in the build directory.
    const std::filesystem::path cmd = root / exe_name;

    // Path to the output CSV file (ensure the directory exists).
    const std::filesystem::path output = root / "convergence_output" / "output.csv";

    // Create the output directory if it doesn't exist.
    std::filesystem::create_directories(output.parent_path());

    // Capture the output of the convergence executable.
    capture_output(cmd.string(), output.string());

    std::cout << "Convergence output saved to: " << output.string() << std::endl;
    return 0;
}