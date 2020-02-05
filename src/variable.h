#ifndef CPQ_VARIABLE_H
#define CPQ_VARIABLE_H

#include <string>

namespace cpq {
class Variable {
public:
  // Default constructor only for Bison nonterminal construction
  Variable() : name() {} ;
  Variable(std::string name) : name(std::move(name)) {}
  virtual ~Variable() {};

  std::string name;

  static Variable make_temp();
private:
  static int counter;
};
} // namespace cpq

#endif // CPQ_VARIABLE_H
