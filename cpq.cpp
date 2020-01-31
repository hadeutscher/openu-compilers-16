#include <iostream>
#include <experimental/filesystem>
#include <exception>
#include <stdexcept>
#include "cpq.tab.hpp"

using namespace std;
namespace fs = experimental::filesystem;

extern FILE *yyin;
extern FILE *yyout;

class program_invocation_error : public logic_error {
    using logic_error::logic_error;
};

void printUsage()
{
    cerr << "Usage: cpq <in_file>.ou" << endl;
}

string createOutputFilename(string input_filename)
{
    fs::path path(input_filename);
    if (path.extension() != ".ou") {
        printUsage();
        throw program_invocation_error("invalid input file");
    }
    return path.replace_extension(".qud");
}

void parseArguments(int argc, const char *argv[])
{
    if (argc < 2) {
        printUsage();
        throw program_invocation_error("not enough args");
    }
    string in_file_name(argv[1]);
    yyin = fopen(in_file_name.c_str(), "r");
    if (!yyin) {
        perror("fopen");
        throw runtime_error("unable to open input");
    }
    string out_file_name(createOutputFilename(in_file_name));
    yyout = fopen(out_file_name.c_str(), "w");
    if (!yyout) {
        perror("fopen");
        throw runtime_error("unable to open output");
    }
}

int main(int argc, const char *argv[])
{
    try {
        parseArguments(argc, argv);
        yyparse();
        return 0;
    } catch (const exception& e) {
        cerr << "Exiting due to error" << endl;
    }
}