%{
#include <iostream>
#include <cmath>
#include "lexer.h"
#undef YY_DECL
#define YY_DECL int cpq::cpqFlexLexer::yylex(yy::parser::semantic_type* sem, yy::parser::location_type* loc)
%}
%option c++ noyywrap nounput noinput nodefault batch
%option yyclass="cpq::cpqFlexLexer"
%x COMMENT
%%
break { return yy::parser::token::BREAK; }
case { return yy::parser::token::CASE; }
default { return yy::parser::token::DEFAULT; }
else { return yy::parser::token::ELSE; }
float { return yy::parser::token::FLOAT; }
if { return yy::parser::token::IF; }
input { return yy::parser::token::INPUT; }
int { return yy::parser::token::INT; }
output { return yy::parser::token::OUTPUT; }
static_cast { return yy::parser::token::STATIC_CAST; }
switch { return yy::parser::token::SWITCH; }
while { return yy::parser::token::WHILE; }
\( { return yy::parser::token::LPAREN; }
\) { return yy::parser::token::RPAREN; }
\{ { return yy::parser::token::LCURL; }
\} { return yy::parser::token::RCURL; }
\, { return yy::parser::token::COMMA; }
\: { return yy::parser::token::COLON; }
\; { return yy::parser::token::SEMICOLON; }
\= { return yy::parser::token::ASSIGN; }
"==" { return yy::parser::token::OP_EQ; }
"!=" { return yy::parser::token::OP_NE; }
\< { return yy::parser::token::OP_LT; }
\> { return yy::parser::token::OP_GT; }
">=" { return yy::parser::token::OP_GEQ; }
"<=" { return yy::parser::token::OP_LEQ; }
\+ { return yy::parser::token::OP_ADD; }
\- { return yy::parser::token::OP_SUB; }
\* { return yy::parser::token::OP_MUL; }
\/ { return yy::parser::token::OP_DIV; }
"||" { return yy::parser::token::OP_OR; }
"&&" { return yy::parser::token::OP_AND; }
\! { return yy::parser::token::OP_NOT; }
[[:digit:]]+ { yylval->build<int>(atoi(yytext)); return yy::parser::token::NUM_INT; }
[[:digit:]]+.[[:digit:]]* { yylval->build<float>(atof(yytext)); return yy::parser::token::NUM_FLOAT; }
[[:alpha:]][[:alnum:]]* {
    if (strlen(yytext) > 9) {
        std::cerr << "Error in line " << yylineno << ": text \"" << yytext << "\" too long for ID token" << std::endl;
        yylval->build<std::string>();
        return yy::parser::token::ID;
    } else {
        yylval->build<std::string>(yytext);
        return yy::parser::token::ID;
    }
}
"/*" { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n
<COMMENT>.
[ \t\n]*
. { std::cerr << "Error in line " << yylineno << ": unrecognized token " <<  yytext << std::endl; }
%%
