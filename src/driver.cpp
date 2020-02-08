#include "driver.h"

#include <cassert>
#include <iomanip>
#include <sstream>

namespace cpq {
static std::string address_to_string(int address) {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(4) << address;
    auto str_label = ss.str();
    assert(str_label.length() == 4);
    return str_label;
}

void Driver::write_arg(Label arg) {
    assert(arg.value > 0);

    BackpatchHandle h = out.tellp();
    out << "XXXX"; // Backpatching with non-binary data is fun
    _backpatches.insert({std::move(arg), std::move(h)});
}

void Driver::write_arg(RelativeLabel arg) {
    out << address_to_string(_curr_address + arg.rel);
}

void Driver::write_arg(Variable arg) {
    assert(!arg.name.empty());

    out << arg.name;
}

void Driver::gen_label(Label l) {
    // Convert the current instruction number to 4 character string to save as
    // the label name
    _labels.insert({std::move(l), std::move(address_to_string(_curr_address))});
}

void Driver::backpatch() {
    for (const auto &[label, bp_handle] : _backpatches) {
        auto ser_label = _labels.at(label);
        auto pos = out.tellp();
        out.seekp(bp_handle);
        out << ser_label;
        out.seekp(pos);
    }
}
} // namespace cpq
