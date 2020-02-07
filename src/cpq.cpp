#include "cpq.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

#include "cpq.tab.hpp"
#include "driver.h"
#include "lexer.h"

static constexpr char watermark[] = "Created by Yuval Deutscher";

static void printUsage() {
    std::cerr << "Usage: cpq [-d] <in_file>.ou" << std::endl;
}

static std::string createOutputFilename(std::string input_filename) {
    std::filesystem::path path(input_filename);
    if (path.extension() != ".ou") {
        printUsage();
        throw program_invocation_error("invalid input file");
    }
    return path.replace_extension(".qud");
}

static auto parseArguments(cpq::Driver &driver, int argc, const char *argv[]) {
    bool debug = false;
    std::optional<std::string> in_file_name = std::nullopt;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-d") {
            debug = true;
        } else if (!in_file_name) {
            in_file_name = arg;
        } else {
            printUsage();
            throw program_invocation_error("too many args");
        }
    }
    if (!in_file_name) {
        printUsage();
        throw program_invocation_error("not enough args");
    }
    driver.in.open(in_file_name.value());
    std::string out_file_name(createOutputFilename(in_file_name.value()));
    driver.out.open(out_file_name);
    return std::make_tuple(debug, in_file_name.value(), out_file_name);
}

int main(int argc, const char *argv[]) {
    bool debug;
    std::string in_file_name{}, out_file_name{};
    bool success = false;
    try {
        cpq::Driver driver;
        std::tie(debug, in_file_name, out_file_name) =
            parseArguments(driver, argc, argv);
        std::cerr << watermark << std::endl;
        cpq::Lexer lexer(driver.in);
        cpq::Parser parse(lexer, driver);
        if (debug) {
            parse.set_debug_level(true);
        }
        if (!parse.parse()) {
            driver.backpatch();
            driver.out << watermark << std::endl;
            success = true;
        }
    } catch (const std::exception &e) {
        std::cerr << "Stopping due to error: " << e.what() << std::endl;
        success = false;
    }
    if (!success) {
        remove(out_file_name.c_str());
        std::cerr << "Exited with no output due to error." << std::endl;
    }
}
