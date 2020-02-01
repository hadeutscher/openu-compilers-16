#ifndef CPQ_H
#define CPQ_H
#include <exception>
#include <stdexcept>
#include <unordered_map>

#include "label.h"

class program_invocation_error : public std::logic_error {
    using std::logic_error::logic_error;
};

namespace cpq 
{
    using BackpatchHandle = int;

    void write_str(std::string s);

    class CodeGenerator {
    public:
        CodeGenerator() : _curr_address(1), _success(true) {}

        template <typename... Args>
        void gen(std::string op, Args... args)
        {
            write_str(move(op));
            auto foo = { write_arg(move(args))... };
        }

        void gen_label(Label l);
        void backpatch();
        void on_error() { _success = false; };
        bool success() { return _success; }

    private:
        void write_arg(std::string arg);
        void write_arg(Label arg);

        int _curr_address;
        bool _success;
        std::unordered_multimap<Label, BackpatchHandle> _backpatches;
        std::unordered_map<Label, SerializedLabel> _labels;
    };

    static CodeGenerator CPQ;
}
#endif //CPQ_H
