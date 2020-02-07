#ifndef CPQ_OPCODES_H
#define CPQ_OPCODES_H

#include <string>

namespace cpq {
enum class Opcode { IASN, IPRT, IINP, IEQL, INQL, ILSS, IGRT, IADD, ISUB, IMLT, IDIV, RASN, RPRT, RINP, REQL, RNQL, RLSS, RGRT, RADD, RSUB, RMLT, RDIV, ITOR, RTOI, JUMP, JMPZ, HALT };

std::string get_opcode_name(Opcode op);
} // end namespace cpq
#endif // CPQ_OPCODES_H
