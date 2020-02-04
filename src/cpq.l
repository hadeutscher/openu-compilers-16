%{
#include <iostream>
#include <cmath>
#include "lexer.h"
#undef YY_DECL
#define YY_DECL yy::parser::symbol_type cpq::cpqFlexLexer::yylex2()
#define yyterminate() return(yy::parser::make_END(loc));
%}
%option c++ noyywrap nounput noinput nodefault batch
%option yyclass="cpq::cpqFlexLexer"
%x COMMENT
%%
break { return yy::parser::make_BREAK(loc); }
case { return yy::parser::make_CASE(loc); }
default { return yy::parser::make_DEFAULT(loc); }
else { return yy::parser::make_ELSE(loc); }
float { return yy::parser::make_FLOAT(loc); }
if { return yy::parser::make_IF(loc); }
input { return yy::parser::make_INPUT(loc); }
int { return yy::parser::make_INT(loc); }
output { return yy::parser::make_OUTPUT(loc); }
static_cast { return yy::parser::make_STATIC_CAST(loc); }
switch { return yy::parser::make_SWITCH(loc); }
while { return yy::parser::make_WHILE(loc); }
\( { return yy::parser::make_LPAREN(loc); }
\) { return yy::parser::make_RPAREN(loc); }
\{ { return yy::parser::make_LCURL(loc); }
\} { return yy::parser::make_RCURL(loc); }
\, { return yy::parser::make_COMMA(loc); }
\: { return yy::parser::make_COLON(loc); }
\; { return yy::parser::make_SEMICOLON(loc); }
\= { return yy::parser::make_ASSIGN(loc); }
"==" { return yy::parser::make_OP_EQ(loc); }
"!=" { return yy::parser::make_OP_NE(loc); }
\< { return yy::parser::make_OP_LT(loc); }
\> { return yy::parser::make_OP_GT(loc); }
">=" { return yy::parser::make_OP_GEQ(loc); }
"<=" { return yy::parser::make_OP_LEQ(loc); }
\+ { return yy::parser::make_OP_ADD(loc); }
\- { return yy::parser::make_OP_SUB(loc); }
\* { return yy::parser::make_OP_MUL(loc); }
\/ { return yy::parser::make_OP_DIV(loc); }
"||" { return yy::parser::make_OP_OR(loc); }
"&&" { return yy::parser::make_OP_AND(loc); }
\! { return yy::parser::make_OP_NOT(loc); }
[[:digit:]]+ { return yy::parser::make_NUM_INT(atoi(yytext), loc); }
[[:digit:]]+.[[:digit:]]* { return yy::parser::make_NUM_FLOAT(atof(yytext), loc); }
[[:alpha:]][[:alnum:]]* {
    if (strlen(yytext) > 9) {
        std::cerr << "Error in line " << yylineno << ": text \"" << yytext << "\" too long for ID token" << std::endl;
        return yy::parser::make_ID(std::string(), loc);
    } else {
        return yy::parser::make_ID(std::string(yytext), loc);
    }
}
"/*" { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n
<COMMENT>.
[ \t\n]*
. { std::cerr << "Error in line " << yylineno << ": unrecognized token " <<  yytext << std::endl; }
%%
