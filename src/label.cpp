#include "label.h"
#include <sstream>

int cpq::Label::counter = 1;

namespace cpq {
Label Label::make_temp() {
    return Label(counter++);
}
} // namespace cpq
