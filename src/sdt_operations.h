#ifndef CPQ_SDT_OPERATIONS_H
#define CPQ_SDT_OPERATIONS_H

#include "sdt_types.h"
#include "driver.h"
#include "cpq.tab.hpp"
#include "lexer.h"

namespace cpq {
Type consolidate_types(Type a, Type b) { return a == b ? a : Type::Float; }

Variable cast_if_needed(Driver& driver, Variable var, Type old_type, Type new_type) {
    if (old_type == new_type) {
        return var;
    }
    auto result = Variable::make_temp();
    switch(new_type) {
    case Type::Int:
        driver.gen("RTOI", result, var);
        break;
    case Type::Float:
        driver.gen("ITOR", result, var);
        break;
    }
    return result;
}

Expression gen_arithmetic_op_expr(Driver& driver, std::string intop, std::string realop, Expression exp_1, Expression exp_2) {
    Expression result(consolidate_types(exp_1.type, exp_2.type));
    auto a = cast_if_needed(driver, exp_1.var, exp_1.type, result.type);
    auto b = cast_if_needed(driver, exp_2.var, exp_2.type, result.type);
    switch (result.type) {
    case Type::Int:
        driver.gen("IMUL", result.var, a, b);
        break;
    case Type::Float:
        driver.gen("RMUL", result.var, a, b);
        break;
    }
    return result;
}

Type get_var_type_or_error(Driver& driver, Lexer& lexer, std::string var) {
    auto type = driver.env().try_get(var);
    if (!type) {
        throw cpq::Parser::syntax_error(lexer.loc, "unknown identifier " + var);
    }
    return type.value();
}
} // namespace cpq
#endif // CPQ_SDT_OPERATIONS_H
