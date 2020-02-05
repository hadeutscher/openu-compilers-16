#ifndef CPQ_DRIVER_H
#define CPQ_DRIVER_H
#include "environment.h"
#include "label.h"
#include "variable.h"
#include <fstream>
#include <unordered_map>

namespace cpq {
using BackpatchHandle = int;

class Driver {
  public:
    Driver() : in(), out(), _env(), _curr_address(1), _success(true) {}
    virtual ~Driver() {}

    template <typename... Args> void gen(std::string op, Args&&... args) {
        out << std::move(op) << " ";
        auto foo = {write_spaced_arg(std::forward<Args>(args))...};
        out << std::endl;
    }

    template <typename Arg> int write_spaced_arg(Arg&& arg) {
        out << " ";
        write_arg(std::forward<Arg>(arg));
        return 0; // Hack to make variadic templates work here
    }
    
    void gen(std::string op) { out << std::move(op) << std::endl; }

    void gen_label(Label l);
    void backpatch();
    void on_error() { _success = false; };
    bool success() const { return _success; }

    Environment& env() { return _env; }
    const Environment& env() const { return _env; }

    std::ifstream in;
    std::ofstream out;

  private:
    void write_arg(std::string arg);
    void write_arg(int arg);
    void write_arg(float arg);
    void write_arg(Label arg);
    void write_arg(Variable arg);

    int _curr_address;
    bool _success;
    std::unordered_multimap<Label, BackpatchHandle> _backpatches;
    std::unordered_map<Label, SerializedLabel> _labels;
    Environment _env;
};
} // namespace cpq

#endif // CPQ_DRIVER_H
