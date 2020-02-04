#ifndef CPQ_SDT_TYPES_H
#define CPQ_SDT_TYPES_H

namespace cpq {
enum class Type { Invalid = 0, Int, Float };

struct Expression {
    cpq::Type type;
};
} // namespace cpq

#endif // CPQ_SDT_TYPES_H
