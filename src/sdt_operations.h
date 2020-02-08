#ifndef CPQ_SDT_OPERATIONS_H
#define CPQ_SDT_OPERATIONS_H

#include "cpq.tab.hpp"
#include "driver.h"
#include "lexer.h"
#include "opcodes.h"
#include "sdt_types.h"

#include <sstream>

namespace cpq {
Variable cast_if_needed(Driver &driver, Variable var, Type old_type,
                        Type new_type);

Expression gen_arithmetic_op_expr(Driver &driver, Opcode intop, Opcode realop,
                                  Expression exp_1, Expression exp_2);

void gen_boolean_op(Driver &driver, ControlFlow flow, Opcode intop,
                    Opcode realop, Expression exp_1, Expression exp_2);

Type get_var_type_or_error(Driver &driver, std::string var);

template <typename T>
std::string immediate_to_string(T x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
}
} // namespace cpq
#endif // CPQ_SDT_OPERATIONS_H
