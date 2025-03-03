#include "capture_output.hpp"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>

/// @brief Runs the executable and redirects its output to the specified file.
/// @param cmd the executable path
/// @param outputFile redirect output from the executable to this file
void capture_output(const std::string& cmd, const std::string& outputFile) {
    std::filesystem::path outputFilePath = outputFile;
    std::filesystem::path cmdPath = cmd;
    outputFilePath = outputFilePath.lexically_normal();

    #if defined(_WIN32) || defined(_WIN64)
    // Use cmd.exe /C so that the redirection is handled correctly
    std::string fullCmd = "cmd.exe /C \"" + cmdPath.string() + "\" > \"" + outputFilePath.string() + "\"";
#else
    std::string fullCmd = "\"" + cmdPath.string() + "\" > \"" + outputFilePath.string() + "\" 2>&1";
#endif


    int ret = std::system(fullCmd.c_str());

    if (ret != 0)
        throw std::runtime_error("capture_output: Error running the program.");
}

/// @brief Runs the executable by redirecting input from one file and output to another.
/// @param cmd the executable path
/// @param inputFile redirect input to the executable from this file
/// @param outputFile redirect output from the executable to this file
void capture_output(const std::string& cmd, const std::string& inputFile, const std::string& outputFile) {
    std::filesystem::path inputFilePath = inputFile;
    std::filesystem::path outputFilePath = outputFile;
    std::filesystem::path cmdPath = cmd;

    inputFilePath = inputFilePath.lexically_normal();
    outputFilePath = outputFilePath.lexically_normal();

    #if defined(_WIN32) || defined(_WIN64)
    std::string fullCmd = "cmd.exe /C \"" + cmdPath.string() + "\" < \"" + inputFilePath.string() + "\" > \"" + outputFilePath.string() + "\"";
#else
    std::string fullCmd = "\"" + cmdPath.string() + "\" < \"" + inputFilePath.string() + "\" > \"" + outputFilePath.string() + "\" 2>&1";
#endif


    int ret = std::system(fullCmd.c_str());

    if (ret != 0)
        throw std::runtime_error("capture_output: Error running the program.");
}