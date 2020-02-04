#ifndef CPQ_VARIABLE_H
#define CPQ_VARIABLE_H

#include <string>

namespace cpq {
class Variable {
public:
  Variable();
  Variable(std::string name) : _name(name) {}
  virtual ~Variable() {};

  std::string name() const { return _name; }
private:
  std::string _name;
  static int counter;
};
} // namespace cpq

#endif // CPQ_VARIABLE_H
