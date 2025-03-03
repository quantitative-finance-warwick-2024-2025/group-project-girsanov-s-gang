#include <filesystem>
#include <iostream>
#include "capture_output.hpp"
#include <string>

int main(int argc, char* argv[])
{
    std::filesystem::path output;
    if (argc >= 3 && std::string(argv[1]) == "--output") {
        output = argv[2];  // Use the provided output path.
    } else {
        // Fallback default.
        const std::filesystem::path root = std::filesystem::current_path();
        output = root / "convergence_output" / "output.csv";
    }
    
    // Debug: print the output path.
    std::cout << "Output file will be: " << output << std::endl;

    // Determine the path to option_pricing_convergence.
    const std::filesystem::path root = std::filesystem::current_path();
    #if defined(_WIN32) || defined(_WIN64)
      const std::filesystem::path exe_name = "option_pricing_convergence.exe";
    #else
      const std::filesystem::path exe_name = "option_pricing_convergence";
    #endif
    const std::filesystem::path cmd = root / exe_name;
    
    // Debug: print the command.
    std::cout << "Using convergence executable at: " << cmd << std::endl;

    // Ensure the output directory exists.
    std::filesystem::create_directories(output.parent_path());

    try {
        capture_output(cmd.string(), output.string());
        std::cout << "Convergence output saved to: " << output << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error capturing output: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}