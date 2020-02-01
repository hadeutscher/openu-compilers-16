#include "cpq_sdd.h"

#include <cassert>

namespace cpq
{
    Type Environment::tryGet(string var_name)
    {
        auto it = sym_tab.find(var_name);
        if (it == sym_tab.end()) {
            return Type::Invalid;
        } else {
            return it->second;
        }
    }

    bool Environment::insert(string var_name, cpq::Type var_type)
    {
        assert(var_type != Type::Invalid);

        auto result = sym_tab.insert({var_name, var_type});
        return result.second;
    }

    int Label::counter = 0;

    void CodeGenerator::write_arg(string arg)
    {
        write_str(move(arg));
    }

    void CodeGenerator::write_arg(Label arg)
    {
        BackpatchHandle h = ftell(yyout);
        write_str("XXXX"); // Backpatching non-binary data is fun
    }

    void CodeGenerator::write_str(string s)
    {
        if (fwrite(s.c_str(), sizeof(*s.c_str()), s.length(), yyout) != s.length()) {
            throw runtime_error("I/O error");
        }
    }
}