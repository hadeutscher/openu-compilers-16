#ifndef CPQ_SDT_TYPES_H
#define CPQ_SDT_TYPES_H

#include <functional>

#include "label.h"
#include "variable.h"

namespace cpq {
enum class Type { Int, Float };

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
} // namespace cpq

#endif // CPQ_SDT_TYPES_H
