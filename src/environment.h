#ifndef CPQ_ENVIRONMENT_H
#define CPQ_ENVIRONMENT_H

#include <unordered_map>
#include <stack>

namespace cpq
{
    enum class Type {
        Invalid = 0,
        Int,
        Float
    };

    class Environment {
        using SymbolTable = std::unordered_map<std::string, Type>;
    public:
        Environment() : sym_tab() {}
        Type tryGet(std::string var_name);
        bool insert(std::string var_name, Type var_type);
    private:
        SymbolTable sym_tab;
    };

    static std::stack<Environment> Environments;
}

#endif //CPQ_ENVIRONMENT_H
