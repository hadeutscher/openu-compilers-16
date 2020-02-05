#include "variable.h"

#include <sstream>

int cpq::Variable::counter = 1;

namespace cpq {
Variable Variable::make_temp() {
    std::stringstream ss;
    ss << "var_" << counter++;
    return Variable(ss.str());
}
} // namespace cpq
