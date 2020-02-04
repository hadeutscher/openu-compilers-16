#include "cpq.h"

#include <experimental/filesystem>
#include <fstream>
#include <iostream>

#include "cpq.tab.hpp"
#include "driver.h"
#include "lexer.h"

namespace fs = std::experimental::filesystem;

static constexpr char watermark[] = "Created by Yuval Deutscher";

static void printUsage() {
    std::cerr << "Usage: cpq <in_file>.ou" << std::endl;
}

static std::string createOutputFilename(std::string input_filename) {
    fs::path path(input_filename);
    if (path.extension() != ".ou") {
        printUsage();
        throw program_invocation_error("invalid input file");
    }
    return path.replace_extension(".qud");
}

static auto parseArguments(cpq::Driver &driver, int argc, const char *argv[]) {
    if (argc < 2) {
        printUsage();
        throw program_invocation_error("not enough args");
    }
    std::string in_file_name(argv[1]);
    driver.in.open(in_file_name);
    std::string out_file_name(createOutputFilename(in_file_name));
    driver.out.open(out_file_name);
    return std::make_tuple(in_file_name, out_file_name);
}

int main(int argc, const char *argv[]) {
    std::string in_file_name{}, out_file_name{};
    bool success;
    try {
        cpq::Driver driver;
        std::tie(in_file_name, out_file_name) =
            parseArguments(driver, argc, argv);
        std::cerr << watermark << std::endl;
        cpq::Lexer lexer(driver.in);
        cpq::Parser parse(lexer, driver);
        if (parse.parse()) {
            driver.on_error();
        }
        driver.backpatch();
        driver.out << watermark << std::endl;
        success = driver.success();
    } catch (const std::exception &e) {
        std::cerr << "Stopping due to error: " << e.what() << std::endl;
        success = false;
    }
    if (!success) {
        remove(out_file_name.c_str());
        std::cerr << "Exited with no ouput due to error." << std::endl;
    }
}
