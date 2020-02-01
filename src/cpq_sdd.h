#ifndef CPQ_CPQ_SDD_H
#define CPQ_CPQ_SDD_H

#include <iostream>
#include <unordered_map>
#include <stack>

using namespace std;

namespace cpq
{
    enum class Type {
        Invalid = 0,
        Int,
        Float
    };

    class Environment {
        using SymbolTable = unordered_map<string, Type>;
    public:
        Environment() : sym_tab() {}
        Type tryGet(string var_name);
        bool insert(string var_name, Type var_type);
    private:
        SymbolTable sym_tab;
    };

    static stack<Environment> Environments;

    using BackpatchHandle = int;

    struct Operand {
        optional<BackpatchHandle> operator() { return auto(); }
    };

    class CodeGenerator {
    public:

    private:
        list<
    };

    static CodeGenerator CPQ;
}
#endif //CPQ_CPQ_SDD_H
