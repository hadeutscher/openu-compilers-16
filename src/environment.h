#ifndef CPQ_ENVIRONMENT_H
#define CPQ_ENVIRONMENT_H

#include <stack>
#include <unordered_map>

#include "sdt_types.h"

namespace cpq {

class Environment {
    using SymbolTable = std::unordered_map<std::string, Type>;

  public:
    Environment() : sym_tab() {}
    virtual ~Environment() {}
    Type try_get(std::string var_name);
    bool insert(std::string var_name, Type var_type);

  private:
    SymbolTable sym_tab;
};

static std::stack<Environment> Environments;
} // namespace cpq

#endif // CPQ_ENVIRONMENT_H
