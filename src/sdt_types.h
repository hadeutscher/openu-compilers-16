#ifndef CPQ_SDT_TYPES_H
#define CPQ_SDT_TYPES_H

#include "label.h"
#include "variable.h"

namespace cpq {
enum class Type { Invalid = 0, Int, Float };

struct Expression {
    Expression() : type(Type::Invalid), var() {}
    Expression(Type type) : type(type), var() {}
    Expression(Type type, Variable var) : type(type), var(var) {}
    virtual ~Expression() {}

    Type type;
    Variable var;
};
} // namespace cpq

#endif // CPQ_SDT_TYPES_H
