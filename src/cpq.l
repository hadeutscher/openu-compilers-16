%{
#include <iostream>
#include <cmath>
#include "cpq.tab.hpp"

using namespace std;
%}
%option noyywrap
%x COMMENT
%%
break { return BREAK; }
case { return CASE; }
default { return DEFAULT; }
else { return ELSE; }
float { return FLOAT; }
if { return IF; }
input { return INPUT; }
int { return INT; }
output { return OUTPUT; }
static_cast { return STATIC_CAST; }
switch { return SWITCH; }
while { return WHILE; }
\( { return LPAREN; }
\) { return RPAREN; }
\{ { return LCURL; }
\} { return RCURL; }
\, { return COMMA; }
\: { return COLON; }
\; { return SEMICOLON; }
\= { return ASSIGN; }
"==" { return OP_EQ; }
"!=" { return OP_NE; }
\< { return OP_LT; }
\> { return OP_GT; }
">=" { return OP_GEQ; }
"<=" { return OP_LEQ; }
\+ { return OP_ADD; }
\- { return OP_SUB; }
\* { return OP_MUL; }
\/ { return OP_DIV; }
"||" { return OP_OR; }
"&&" { return OP_AND; }
\! { return OP_NOT; }
[[:digit:]]+ { yylval.num_int.number = atoi(yytext); return NUM_INT; }
[[:digit:]]+.[[:digit:]]* { yylval.num_float.number = atof(yytext); return NUM_FLOAT; }
[[:alpha:]][[:alnum:]]* {
    if (strlen(yytext) > 9) {
        cerr << "Error in line " << yylineno << ": text \"" << yytext << "\" too long for ID token" << endl;
        yylval.id.name = "";
        return ID;
    } else {
        yylval.id.name = yytext;
        return ID;
    }
}
"/*" { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n
<COMMENT>.
[ \t\n]*
. { cerr << "Error in line " << yylineno << ": unrecognized token " <<  yytext << endl; }
%%
