%{
#include <iostream>
#include <cmath>
#include "lexer.h"
#undef YY_DECL
#define YY_DECL cpq::Parser::symbol_type cpq::Lexer::next_token()
#define yyterminate() return(cpq::Parser::make_END(loc));
#define YY_USER_ACTION loc.columns(yyleng);
%}
%option c++ noyywrap nounput noinput batch
%option yyclass="cpq::Lexer"
%x COMMENT
%%
%{
    loc.step();
%}
break { return cpq::Parser::make_BREAK(loc); }
case { return cpq::Parser::make_CASE(loc); }
default { return cpq::Parser::make_DEFAULT(loc); }
else { return cpq::Parser::make_ELSE(loc); }
float { return cpq::Parser::make_FLOAT(loc); }
if { return cpq::Parser::make_IF(loc); }
input { return cpq::Parser::make_INPUT(loc); }
int { return cpq::Parser::make_INT(loc); }
output { return cpq::Parser::make_OUTPUT(loc); }
static_cast { return cpq::Parser::make_STATIC_CAST(loc); }
switch { return cpq::Parser::make_SWITCH(loc); }
while { return cpq::Parser::make_WHILE(loc); }
\( { return cpq::Parser::make_LPAREN(loc); }
\) { return cpq::Parser::make_RPAREN(loc); }
\{ { return cpq::Parser::make_LCURL(loc); }
\} { return cpq::Parser::make_RCURL(loc); }
\, { return cpq::Parser::make_COMMA(loc); }
\: { return cpq::Parser::make_COLON(loc); }
\; { return cpq::Parser::make_SEMICOLON(loc); }
\= { return cpq::Parser::make_ASSIGN(loc); }
"==" { return cpq::Parser::make_OP_EQ(loc); }
"!=" { return cpq::Parser::make_OP_NE(loc); }
\< { return cpq::Parser::make_OP_LT(loc); }
\> { return cpq::Parser::make_OP_GT(loc); }
">=" { return cpq::Parser::make_OP_GEQ(loc); }
"<=" { return cpq::Parser::make_OP_LEQ(loc); }
\+ { return cpq::Parser::make_OP_ADD(loc); }
\- { return cpq::Parser::make_OP_SUB(loc); }
\* { return cpq::Parser::make_OP_MUL(loc); }
\/ { return cpq::Parser::make_OP_DIV(loc); }
"||" { return cpq::Parser::make_OP_OR(loc); }
"&&" { return cpq::Parser::make_OP_AND(loc); }
\! { return cpq::Parser::make_OP_NOT(loc); }
[[:digit:]]+ { return cpq::Parser::make_NUM_INT(atoi(yytext), loc); }
[[:digit:]]+\.[[:digit:]]* { return cpq::Parser::make_NUM_FLOAT(atof(yytext), loc); }
[[:alpha:]_][[:alnum:]_]* {
    if (strlen(yytext) > 9) {
        std::cerr << "Error in line " << yylineno << ": text \"" << yytext << "\" too long for ID token" << std::endl;
        return cpq::Parser::make_ID(std::string(), loc);
    } else {
        return cpq::Parser::make_ID(std::string(yytext), loc);
    }
}
[ \t\r]+ { loc.step(); }
\n+ { loc.lines(yyleng); loc.step(); }
"/*" { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>.
. { throw cpq::Parser::syntax_error(loc, "unrecognized token " + std::string(yytext)); }
%%
