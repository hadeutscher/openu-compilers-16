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

}