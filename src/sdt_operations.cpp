#include "sdt_operations.h"

#include "cpq.h"

namespace cpq {
static Type consolidate_types(Type a, Type b) {
    return a == b ? a : Type::Float;
}

Variable cast_if_needed(Driver &driver, Variable var, Type old_type,
                        Type new_type) {
    if (old_type == new_type) {
        return var;
    }
    auto result = Variable::make_temp();
    switch (new_type) {
    case Type::Int:
        driver.gen(Opcode::RTOI, result, var);
        break;
    case Type::Float:
        driver.gen(Opcode::ITOR, result, var);
        break;
    }
    return result;
}

Expression gen_arithmetic_op_expr(Driver &driver, Opcode intop, Opcode realop,
                                  Expression exp_1, Expression exp_2) {
    Expression result(consolidate_types(exp_1.type, exp_2.type), Variable::make_temp());
    auto a = cast_if_needed(driver, exp_1.var, exp_1.type, result.type);
    auto b = cast_if_needed(driver, exp_2.var, exp_2.type, result.type);
    switch (result.type) {
    case Type::Int:
        driver.gen(intop, result.var, a, b);
        break;
    case Type::Float:
        driver.gen(realop, result.var, a, b);
        break;
    }
    return result;
}

void gen_boolean_op(Driver &driver, ControlFlow flow, Opcode intop,
                    Opcode realop, Expression exp_1, Expression exp_2) {
    auto common_type = consolidate_types(exp_1.type, exp_2.type);
    auto result = Variable::make_temp();
    auto a = cast_if_needed(driver, exp_1.var, exp_1.type, common_type);
    auto b = cast_if_needed(driver, exp_2.var, exp_2.type, common_type);
    switch (common_type) {
    case Type::Int:
        driver.gen(intop, result, a, b);
        break;
    case Type::Float:
        driver.gen(realop, result, a, b);
        break;
    }

    if (flow.ctrl_true && flow.ctrl_false) {
        // Both are jumps, simply generate true/false jump code
        driver.gen(Opcode::JMPZ, flow.ctrl_false.value(), result);
        driver.gen(Opcode::JUMP, flow.ctrl_true.value());
    } else if (flow.ctrl_false && !flow.ctrl_true) {
        // True flow is fallthrough, this is simple since we have ifFalse
        driver.gen(Opcode::JMPZ, flow.ctrl_false.value(), result);
    } else if (!flow.ctrl_false && flow.ctrl_true) {
        // False flow is fallthrough; since we have no ifTrue instruction, we
        // have to generate a logical NOT here
        driver.gen(Opcode::IEQL, result, result, 0);
        driver.gen(Opcode::JMPZ, flow.ctrl_true.value(), result);
    } else {
        assert(false);
    }
}

Type get_var_type_or_error(Driver &driver, std::string var) {
    auto type = driver.env().try_get(var);
    if (!type) {
        throw syntax_error_wrapper("unknown identifier " + var);
    }
    return type.value();
}
} // namespace cpq
