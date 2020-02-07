#include "sdt_types.h"

#include "driver.h"
#include "sdt_operations.h"
#include <cassert>

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
        gen_boolean_op(driver, std::move(flow), Opcode::IEQL, Opcode::REQL, a,
                       b);
        return;
    case CompareOperation::NotEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::INQL, Opcode::RNQL, a,
                       b);
        return;
    case CompareOperation::LessThan:
        gen_boolean_op(driver, std::move(flow), Opcode::ILSS, Opcode::RLSS, a,
                       b);
        return;
    case CompareOperation::GreaterThan:
        gen_boolean_op(driver, std::move(flow), Opcode::IGRT, Opcode::RGRT, a,
                       b);
        return;
    case CompareOperation::LessEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::IGRT, Opcode::RGRT, b,
                       a);
        return;
    case CompareOperation::GreaterEqual:
        gen_boolean_op(driver, std::move(flow), Opcode::ILSS, Opcode::RLSS, b,
                       a);
        return;
    }
    assert(false);
}
} // namespace cpq
