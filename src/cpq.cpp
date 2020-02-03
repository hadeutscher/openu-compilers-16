#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <FlexLexer.h>
#include "cpq.tab.hpp"
#include "cpq.h"

namespace fs = std::experimental::filesystem;

std::ifstream yyin{};
std::ofstream yyout{};

namespace cpq
{
    void write_str(std::string s)
    {
        // TODO
    }

    void CodeGenerator::write_arg(std::string arg)
    {
        write_str(move(arg));
    }

    void CodeGenerator::write_arg(Label arg)
    {
        BackpatchHandle h = 0; // TODO
        write_str("XXXX"); // Backpatching with non-binary data is fun
        _backpatches.insert({std::move(arg), std::move(h)});
    }

    void CodeGenerator::gen_label(Label l)
    {
        // Convert the current instruction number to 4 character string to save as the label name
        std::ostringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(4) << _curr_address;
        auto str_label = ss.str();
        assert(str_label.length() == 4);
        _labels.insert({std::move(l), std::move(str_label)});
    }

    static void seek_output(long int off)
    {
        // TODO
    }

    void CodeGenerator::backpatch()
    {
        for (const auto &[label, bp_handle] : _backpatches) {
            auto ser_label = _labels.at(label);
            auto pos = 0; // TODO
            seek_output(bp_handle);
            write_str(ser_label);
            seek_output(pos);
        }
    }
} // namespace cpq

static constexpr char watermark[] = "Created by Yuval Deutscher";

static void printUsage()
{
    std::cerr << "Usage: cpq <in_file>.ou" << std::endl;
}

static std::string createOutputFilename(std::string input_filename)
{
    fs::path path(input_filename);
    if (path.extension() != ".ou") {
        printUsage();
        throw program_invocation_error("invalid input file");
    }
    return path.replace_extension(".qud");
}

static auto parseArguments(int argc, const char *argv[])
{
    if (argc < 2) {
        printUsage();
        throw program_invocation_error("not enough args");
    }
    std::string in_file_name(argv[1]);
    yyin.open(in_file_name);
    std::string out_file_name(createOutputFilename(in_file_name));
    yyout.open(out_file_name); 
}

int main(int argc, const char *argv[])
{
    std::string in_file_name{}, out_file_name{};
    try {
        //std::tie(in_file_name, yyin, out_file_name, yyout) = parseArguments(argc, argv);
        parseArguments(argc, argv);
        std::cerr << watermark << std::endl;
        yyFlexLexer lexer(yyin, yyout);
        yy::parser parse(lexer);
        if (parse.parse()) {
            cpq::CPQ.on_error();
        }
        cpq::CPQ.backpatch();
        cpq::write_str(watermark);
        cpq::write_str("\n");
    } catch (const std::exception& e) {
        cpq::CPQ.on_error();
        std::cerr << "Stopping due to error: " << e.what() << std::endl;
    }
    //yyin.close();
    //yyout.close();
    if (!cpq::CPQ.success()) {
        remove(out_file_name.c_str());
        std::cerr << "Exited with no ouput due to error." << std::endl;
    }
}
