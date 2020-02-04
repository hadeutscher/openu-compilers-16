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
   using FlexLexer::yylex;

   virtual
   int yylex( yy::parser::semantic_type * const lval, 
              yy::parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   yy::parser::semantic_type *yylval = nullptr;
};

} /* end namespace cpq */

#endif /* CPQ_LEXER_H */