#ifndef CPQ_SDT_TYPES_H
#define CPQ_SDT_TYPES_H

#include <functional>
#include <optional>

#include "label.h"
#include "variable.h"

namespace cpq {
enum class Type { Int, Float };

static const std::optional<Label> Fallthrough = {};

struct Expression {
    // The default constructor only appears here to allow usage by Bison
    // we never actually use default-constructed expressions
    Expression() : type(Type::Int), var() {}
    Expression(Type type) : type(type), var() {}
    Expression(Type type, Variable var) : type(type), var(var) {}
    virtual ~Expression() {}

    Type type;
    Variable var;
};

struct ControlFlow {
    ControlFlow() : ctrl_true(), ctrl_false() {}
    ControlFlow(std::optional<Label> ctrl_true, std::optional<Label> ctrl_false) : ctrl_true(ctrl_true), ctrl_false(ctrl_false) {}
    virtual ~ControlFlow() {}

    std::optional<Label> ctrl_true;
    std::optional<Label> ctrl_false;
};
} // namespace cpq

#endif // CPQ_SDT_TYPES_H
