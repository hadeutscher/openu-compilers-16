#ifndef CPQ_SDT_TYPES_H
#define CPQ_SDT_TYPES_H

#include <functional>
#include <memory>
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
    Expression(Type type, Variable var) : type(type), var(var) {}
    virtual ~Expression() {}

    Type type;
    Variable var;
};

struct ControlFlow {
    ControlFlow() : ctrl_true(), ctrl_false() {}
    ControlFlow(std::optional<Label> ctrl_true, std::optional<Label> ctrl_false)
        : ctrl_true(ctrl_true), ctrl_false(ctrl_false) {}
    virtual ~ControlFlow() {}

    std::optional<Label> ctrl_true;
    std::optional<Label> ctrl_false;
};

class Driver;

class BooleanNode {
  public:
    virtual ~BooleanNode() {}
    virtual void gen(Driver &driver, ControlFlow flow) = 0;
};

class ExpressionNode {
  public:
    virtual ~ExpressionNode() {}
    virtual Expression gen(Driver &driver) = 0;
};

class BooleanBinaryNode : public BooleanNode {
  public:
    enum class LogicalOperation { And, Or };

    // The default constructor only appears here to allow usage by Bison
    BooleanBinaryNode() : BooleanNode(), a(), b(), op(LogicalOperation::And) {}
    BooleanBinaryNode(std::unique_ptr<BooleanNode> a,
                      std::unique_ptr<BooleanNode> b, LogicalOperation op)
        : BooleanNode(), a(std::move(a)), b(std::move(b)), op(op) {}
    virtual ~BooleanBinaryNode() {}

    std::unique_ptr<BooleanNode> a;
    std::unique_ptr<BooleanNode> b;
    LogicalOperation op;

    virtual void gen(Driver &driver, ControlFlow flow);
};

class BooleanNotNode : public BooleanNode {
  public:
    // The default constructor only appears here to allow usage by Bison
    BooleanNotNode() : BooleanNode(), a() {}
    BooleanNotNode(std::unique_ptr<BooleanNode> a)
        : BooleanNode(), a(std::move(a)) {}
    virtual ~BooleanNotNode() {}

    std::unique_ptr<BooleanNode> a;

    virtual void gen(Driver &driver, ControlFlow flow);
};

class BooleanLeafNode : public BooleanNode {
  public:
    enum class CompareOperation {
        Equal,
        NotEqual,
        LessThan,
        GreaterThan,
        LessEqual,
        GreaterEqual
    };

    // The default constructor only appears here to allow usage by Bison
    BooleanLeafNode() : BooleanNode(), a(), b(), op(CompareOperation::Equal) {}
    BooleanLeafNode(std::unique_ptr<ExpressionNode> a,
                    std::unique_ptr<ExpressionNode> b, CompareOperation op)
        : BooleanNode(), a(std::move(a)), b(std::move(b)), op(op) {}
    virtual ~BooleanLeafNode() {}

    std::unique_ptr<ExpressionNode> a;
    std::unique_ptr<ExpressionNode> b;
    CompareOperation op;

    virtual void gen(Driver &driver, ControlFlow flow);
};

class ExpressionBinaryNode : public ExpressionNode {
  public:
    enum class ArithmeticOperation {
        Add,
        Subtract,
        Multiply,
        Divide,
    };

    // The default constructor only appears here to allow usage by Bison
    ExpressionBinaryNode()
        : ExpressionNode(), a(), b(), op(ArithmeticOperation::Add) {}
    ExpressionBinaryNode(std::unique_ptr<ExpressionNode> a,
                         std::unique_ptr<ExpressionNode> b,
                         ArithmeticOperation op)
        : ExpressionNode(), a(std::move(a)), b(std::move(b)), op(op) {}
    virtual ~ExpressionBinaryNode() {}

    std::unique_ptr<ExpressionNode> a;
    std::unique_ptr<ExpressionNode> b;
    ArithmeticOperation op;

    virtual Expression gen(Driver &driver);
};

class ExpressionLeafNode : public ExpressionNode {
  public:
    ExpressionLeafNode() : ExpressionNode(), e() {}
    ExpressionLeafNode(Expression e) : ExpressionNode(), e(std::move(e)) {}
    virtual ~ExpressionLeafNode() {}

    Expression e;

    virtual Expression gen(Driver &driver);
};
} // namespace cpq

#endif // CPQ_SDT_TYPES_H
