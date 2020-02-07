#ifndef CPQ_LABEL_H
#define CPQ_LABEL_H
#include <string>

namespace cpq {
class Label {
  public:
    // Default constructor only for Bison nonterminal construction
    Label() : value(0) {}
    virtual ~Label() {}
    bool operator==(const Label &other) const { return value == other.value; }

    int value;

    static Label make_temp();

  private:
    // This can be private as we do not support user-defined labels
    Label(int value) : value(value) {}

    static int counter;
};

using SerializedLabel = std::string;
} // namespace cpq

namespace std {
template <> struct hash<cpq::Label> {
    std::size_t operator()(const cpq::Label &l) const {
        return hash<int>()(l.value);
    }
};
} // namespace std
#endif // CPQ_LABEL_H
