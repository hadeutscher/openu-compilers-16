#include "variable.h"

#include <sstream>

int cpq::Variable::counter = 0;

namespace cpq {
Variable::Variable()
{
    std::stringstream ss;
    ss << "var_" << std::hex << counter++;
    _name = ss.str();
}
} // namespace cpq
