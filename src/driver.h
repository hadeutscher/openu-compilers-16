#ifndef CPQ_DRIVER_H
#define CPQ_DRIVER_H
#include "environment.h"
#include "label.h"
#include "opcodes.h"
#include "variable.h"
#include <fstream>
#include <stack>
#include <unordered_map>

namespace cpq {
using BackpatchHandle = int;

class Driver {
  public:
    Driver() : in(), out(), _env(), _curr_address(1) {}
    virtual ~Driver() {}

    template <typename... Args> void gen(Opcode op, Args &&... args) {
        out << get_opcode_name(op) << " ";
        auto foo = {write_spaced_arg(std::forward<Args>(args))...};
        out << std::endl;
        _curr_address++;
    }

    template <typename Arg> int write_spaced_arg(Arg &&arg) {
        out << " ";
        write_arg(std::forward<Arg>(arg));
        return 0; // Hack to make variadic templates work here
    }

    void gen(Opcode op) {
        out << get_opcode_name(op) << std::endl;
        _curr_address++;
    }
    void gen_label(Label l);
    void backpatch();

    Environment &env() { return _env; }
    const Environment &env() const { return _env; }

    Label get_scope_end() const { return _scope_ends.top(); }
    void enter_breakable_scope(Label label) {
        _scope_ends.push(std::move(label));
    }
    Label exit_breakable_scope() {
        auto result = _scope_ends.top();
        _scope_ends.pop();
        return result;
    }

    std::ifstream in;
    std::ofstream out;

  private:
    void write_arg(int arg) { out << arg; }
    void write_arg(float arg) { out << arg; }
    void write_arg(Label arg);
    void write_arg(RelativeLabel arg);
    void write_arg(Variable arg);

    int _curr_address;
    std::unordered_multimap<Label, BackpatchHandle> _backpatches;
    std::unordered_map<Label, SerializedLabel> _labels;
    Environment _env;
    std::stack<Label> _scope_ends;
};
} // namespace cpq

#endif // CPQ_DRIVER_H
