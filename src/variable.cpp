#include "variable.h"

#include <sstream>

int cpq::Variable::counter = 0;

namespace cpq {
std::string Variable::name() {
    if (!_initialized) {
        std::stringstream ss;
        ss << "var_" << std::hex << counter++;
        _name = ss.str();
    }
    return _name;
}
} // namespace cpq
