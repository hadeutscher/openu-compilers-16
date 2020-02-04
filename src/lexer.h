#ifndef CPQ_LEXER_H
#define CPQ_LEXER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "cpq.tab.hpp"
#include "location.hh"

namespace cpq {
class Lexer : public yyFlexLexer {
public:
  Lexer(std::istream &in) : yyFlexLexer(&in){};
  virtual ~Lexer(){};

  Parser::symbol_type next_token();

  cpq::location loc;

private:
  Parser::semantic_type *yylval = nullptr;
};
} // namespace cpq

#endif /* CPQ_LEXER_H */
