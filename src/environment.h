#ifndef CPQ_ENVIRONMENT_H
#define CPQ_ENVIRONMENT_H

#include <functional>
#include <optional>
#include <unordered_map>
#include <vector>

#include "sdt_types.h"

namespace cpq {
class Environment {
    using SymbolTable = std::unordered_map<std::string, Type>;

  public:
    Environment() : _sym_tab() {}
    virtual ~Environment() {}
    std::optional<Type> try_get(std::string var_name);
    bool insert(std::string var_name, Type var_type);

  private:
    SymbolTable _sym_tab;
};
} // namespace cpq

#endif // CPQ_ENVIRONMENT_H
