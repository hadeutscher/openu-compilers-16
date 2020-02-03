%{
#include <iostream>
#include <cmath>
#include "cpq.tab.hpp"
%}
%option c++ noyywrap nounput noinput batch
%x COMMENT
%%
break { return yy::parser::make_BREAK(); }
case { return yy::parser::make_CASE(); }
default { return yy::parser::make_DEFAULT(); }
else { return yy::parser::make_ELSE(); }
float { return yy::parser::make_FLOAT(); }
if { return yy::parser::make_IF(); }
input { return yy::parser::make_INPUT(); }
int { return yy::parser::make_INT(); }
output { return yy::parser::make_OUTPUT(); }
static_cast { return yy::parser::make_STATIC_CAST(); }
switch { return yy::parser::make_SWITCH(); }
while { return yy::parser::make_WHILE(); }
\( { return yy::parser::make_LPAREN(); }
\) { return yy::parser::make_RPAREN(); }
\{ { return yy::parser::make_LCURL(); }
\} { return yy::parser::make_RCURL(); }
\, { return yy::parser::make_COMMA(); }
\: { return yy::parser::make_COLON(); }
\; { return yy::parser::make_SEMICOLON(); }
\= { return yy::parser::make_ASSIGN(); }
"==" { return yy::parser::make_OP_EQ(); }
"!=" { return yy::parser::make_OP_NE(); }
\< { return yy::parser::make_OP_LT(); }
\> { return yy::parser::make_OP_GT(); }
">=" { return yy::parser::make_OP_GEQ(); }
"<=" { return yy::parser::make_OP_LEQ(); }
\+ { return yy::parser::make_OP_ADD(); }
\- { return yy::parser::make_OP_SUB(); }
\* { return yy::parser::make_OP_MUL(); }
\/ { return yy::parser::make_OP_DIV(); }
"||" { return yy::parser::make_OP_OR(); }
"&&" { return yy::parser::make_OP_AND(); }
\! { return yy::parser::make_OP_NOT(); }
[[:digit:]]+ { yylval.num_int.number = atoi(yytext); return yy::parser::make_NUM_INT(); }
[[:digit:]]+.[[:digit:]]* { yylval.num_float.number = atof(yytext); return yy::parser::make_NUM_FLOAT(); }
[[:alpha:]][[:alnum:]]* {
    if (strlen(yytext) > 9) {
        std::cerr << "Error in line " << yylineno << ": text \"" << yytext << "\" too long for ID token" << std::endl;
        yylval.id.name = "";
        return yy::parser::make_ID();
    } else {
        yylval.id.name = yytext;
        return yy::parser::make_ID();
    }
}
"/*" { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n
<COMMENT>.
[ \t\n]*
. { std::cerr << "Error in line " << yylineno << ": unrecognized token " <<  yytext << std::endl; }
%%
