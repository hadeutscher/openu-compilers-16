#include "opcodes.h"
#include <cassert>

namespace cpq {
std::string get_opcode_name(Opcode op) {
    switch (op) {
    {% for op in opcodes %}
    case Opcode::{{ op }}:
        return "{{ op }}";
    {% endfor %}
    }
    assert(false);
}
} // end namespace cpq
