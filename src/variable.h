#ifndef CPQ_VARIABLE_H
#define CPQ_VARIABLE_H

#include <string>

namespace cpq {
class Variable {
public:
  Variable() : _initialized(false), _name() {} ;
  Variable(std::string name) : _initialized(true), _name(name) {}
  virtual ~Variable() {};

  std::string name();
private:
  std::string _name;
  bool _initialized;

  static int counter;
};
} // namespace cpq

#endif // CPQ_VARIABLE_H
