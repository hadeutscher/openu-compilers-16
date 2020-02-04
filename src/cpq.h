#ifndef CPQ_H
#define CPQ_H
#include <exception>
#include <stdexcept>

class program_invocation_error : public std::logic_error {
    using std::logic_error::logic_error;
};

#endif // CPQ_H
