#ifndef CPQ_CPQ_SDD_H
#define CPQ_CPQ_SDD_H

#include <iostream>
#include <unordered_map>
#include <stack>

using namespace std;

extern FILE *yyout;

namespace cpq
{
    class Label {
    public:
        Label() : value(counter++) {}
        bool operator==(const Label &other) const { return value == other.value; }

        int value;
    private:
        static int counter;
    };
}

namespace std
{
    template<>
    struct hash<cpq::Label>
    {
        std::size_t operator()(const cpq::Label& l) const { return hash<int>()(l.value); }
    };
}

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

    using SerializedLabel = int;

    class CodeGenerator {
    public:
        template <typename... Args>
        void gen(string op, Args... args)
        {
            write_str(move(op));
            auto foo = { write_arg(move(args))... };
        }

        void write_arg(string arg);
        void write_arg(Label arg);
        void write_str(string s);
        void gen_label(Label l);

    private:
        unordered_multimap<string, BackpatchHandle> backpatches;
        unordered_map<Label, SerializedLabel> labels;
    };

    static CodeGenerator CPQ;
}

#endif //CPQ_CPQ_SDD_H
