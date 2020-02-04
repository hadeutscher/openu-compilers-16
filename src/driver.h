#ifndef CPQ_DRIVER_H
#define CPQ_DRIVER_H
#include <unordered_map>
#include <fstream>
#include "label.h"

namespace cpq 
{
    using BackpatchHandle = int;

    class Driver {
    public:
        Driver() : in(), out(), _curr_address(1), _success(true) {}
        virtual ~Driver() {}

        template <typename... Args>
        void gen(std::string op, Args... args)
        {
            out << move(op);
            auto foo = { write_arg(move(args))... };
            out << std::endl;
        }

        void gen_label(Label l);
        void backpatch();
        void on_error() { _success = false; };
        bool success() { return _success; }

        std::ifstream in;
        std::ofstream out;
    private:
        void write_arg(std::string arg);
        void write_arg(Label arg);

        int _curr_address;
        bool _success;
        std::unordered_multimap<Label, BackpatchHandle> _backpatches;
        std::unordered_map<Label, SerializedLabel> _labels;
    };
} // namespace cpq

#endif //CPQ_DRIVER_H
