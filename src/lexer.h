#ifndef CPQ_LEXER_H
#define CPQ_LEXER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "cpq.tab.hpp"
#include "location.hh"

namespace cpq {
class cpqFlexLexer : public yyFlexLexer {
public:
   cpqFlexLexer(std::istream& in) : yyFlexLexer(&in)
   {
   };
   virtual ~cpqFlexLexer() {
   };

   //get rid of override virtual function warning
   //using FlexLexer::yylex;

   virtual
   yy::parser::symbol_type yylex2();
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc

    yy::location loc;
private:
   /* yyval ptr */
   yy::parser::semantic_type *yylval = nullptr;
};

} /* end namespace cpq */

#endif /* CPQ_LEXER_H */