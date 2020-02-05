#include "environment.h"

#include <cassert>

namespace cpq {
std::optional<Type> Environment::try_get(std::string var_name) {
    auto it = _sym_tab.find(var_name);
    if (it == _sym_tab.end()) {
        return std::nullopt;
    } else {
        return it->second;
    }
}

bool Environment::insert(std::string var_name, cpq::Type var_type) {
    auto [iter, success] =
        _sym_tab.insert({std::move(var_name), std::move(var_type)});
    return success;
}
} // namespace cpq
