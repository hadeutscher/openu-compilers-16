%{
#include "cpq.tab.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
%}

%option noyywrap

%%

[0-9]+   { yylval.number.val = atoi(yytext); return NUMBER; }
sum      { return SUM; }
zero     { return ZERO; }
height   { return HEIGHT; }
double   { return DOUBLE; }
"("      { return LPAREN; }
")"      { return RPAREN; }
[ \t\n]+ { }
.        { cerr << "Unrecognized token \"" << yytext << "\" in line " << yylineno << endl; exit(1); }
%%
