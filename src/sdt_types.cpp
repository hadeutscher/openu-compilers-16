#include "sdt_types.h"

#include "driver.h"
#include "sdt_operations.h"
#include <cassert>
#include <sstream>

namespace cpq {
void BooleanBinaryNode::gen(Driver &driver, ControlFlow flow) {
    switch (op) {
    case LogicalOperation::And:
        if (flow.ctrl_false) {
            a->gen(driver, ControlFlow(Fallthrough, flow.ctrl_false));
            b->gen(driver, std::move(flow));
        } else {
            // False action is to fallthrough
            auto end_label = Label::make_temp();
            a->gen(driver, ControlFlow(Fallthrough, end_label));
            b->gen(driver, std::move(flow));
            driver.gen_label(end_label);
        }
        return;
    case LogicalOperation::Or:
        if (flow.ctrl_true) {
            a->gen(driver, ControlFlow(flow.ctrl_true, Fallthrough));
            b->gen(driver, std::move(flow));
        } else {
            // True action is to fallthrough
            auto end_label = Label::make_temp();
            a->gen(driver, ControlFlow(end_label, Fallthrough));
            b->gen(driver, std::move(flow));
            driver.gen_label(end_label);
        }
        return;
    }
    assert(false);
}

void BooleanNotNode::gen(Driver &driver, ControlFlow flow) {
    a->gen(driver, ControlFlow(flow.ctrl_false, flow.ctrl_true));
}

void BooleanLeafNode::gen(Driver &driver, ControlFlow flow) {
    switch (op) {
    case CompareOperation::Equal:
        gen_boolean_op(driver, std::move(flow), Opcode::IEQL, Opcode::REQL,
                       a->gen(driver), b->gen(driver));
        return;
    case CompareOperation::NotEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::INQL, Opcode::RNQL,
                       a->gen(driver), b->gen(driver));
        return;
    case CompareOperation::LessThan:
        gen_boolean_op(driver, std::move(flow), Opcode::ILSS, Opcode::RLSS,
                       a->gen(driver), b->gen(driver));
        return;
    case CompareOperation::GreaterThan:
        gen_boolean_op(driver, std::move(flow), Opcode::IGRT, Opcode::RGRT,
                       a->gen(driver), b->gen(driver));
        return;
    case CompareOperation::LessEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::IGRT, Opcode::RGRT,
                       b->gen(driver), a->gen(driver));
        return;
    case CompareOperation::GreaterEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::ILSS, Opcode::RLSS,
                       b->gen(driver), a->gen(driver));
        return;
    }
    assert(false);
}

Expression ExpressionBinaryNode::gen(Driver &driver) {
    switch (op) {
    case ArithmeticOperation::Add:
        return gen_arithmetic_op_expr(driver, Opcode::IADD, Opcode::RADD,
                                      a->gen(driver), b->gen(driver));
    case ArithmeticOperation::Subtract:
        return gen_arithmetic_op_expr(driver, Opcode::ISUB, Opcode::RSUB,
                                      a->gen(driver), b->gen(driver));
    case ArithmeticOperation::Multiply:
        return gen_arithmetic_op_expr(driver, Opcode::IMLT, Opcode::RMLT,
                                      a->gen(driver), b->gen(driver));
    case ArithmeticOperation::Divide:
        return gen_arithmetic_op_expr(driver, Opcode::IDIV, Opcode::RDIV,
                                      a->gen(driver), b->gen(driver));
    }
    assert(false);
}

Expression ExpressionLeafNode::gen(Driver &driver) { return e; }
} // namespace cpq
