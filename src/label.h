#ifndef CPQ_LABEL_H
#define CPQ_LABEL_H
#include <string>

namespace cpq {
class Label {
public:
  Label() : value(counter++) {}
  bool operator==(const Label &other) const { return value == other.value; }

  int value;

private:
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
