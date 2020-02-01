#include <cassert>

#include "environment.h"

extern FILE *yyout;

namespace cpq
{
    Type Environment::tryGet(std::string var_name)
    {
        auto it = sym_tab.find(var_name);
        if (it == sym_tab.end()) {
            return Type::Invalid;
        } else {
            return it->second;
        }
    }

    bool Environment::insert(std::string var_name, cpq::Type var_type)
    {
        assert(var_type != Type::Invalid);

        auto [iter, success] = sym_tab.insert({std::move(var_name), std::move(var_type)});
        return success;
    }
}