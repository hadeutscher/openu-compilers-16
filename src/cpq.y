%skeleton "lalr1.cc"
%require  "3.1"
%{
#include "driver.h"
#include "environment.h"
#include "label.h"
#include "lexer.h"
#include "opcodes.h"
#include "sdt_operations.h"
#include "variable.h"
#include <iostream>
%}
%language "c++"
%defines
%define api.namespace {cpq}
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error verbose
%locations

%code requires {
    #include "sdt_types.h"
    namespace cpq { class Lexer; class Driver; }
}
%parse-param { cpq::Lexer& lexer }
%parse-param { cpq::Driver& driver }

%code {
    #undef yylex
    #define yylex lexer.next_token
}

%token BREAK CASE DEFAULT ELSE FLOAT IF INPUT INT OUTPUT STATIC_CAST SWITCH WHILE
%token LPAREN RPAREN LCURL RCURL COMMA COLON SEMICOLON ASSIGN
%token OP_EQ OP_NE OP_LT OP_GT OP_GEQ OP_LEQ OP_ADD OP_SUB OP_MUL OP_DIV OP_OR OP_AND OP_NOT
%token END 0

%token <int> NUM_INT
%token <float> NUM_FLOAT
%token <std::string> ID

%nterm <Type> type
%nterm <std::vector<std::string>> idlist
%nterm <Expression> expression term factor num
%nterm <std::unique_ptr<BooleanNode>> boolexpr boolterm boolfactor
%%
%start program;
program : declarations stmt_block { driver.gen(Opcode::HALT); } ;

declarations : declarations declaration
             | %empty ;

declaration : idlist COLON type SEMICOLON
{
    for (auto id : $1) {
        driver.env().insert(id, $3);
    }
};

type : INT { $$ = Type::Int; }
     | FLOAT { $$ = Type::Float; }
	 ;

idlist : idlist COMMA ID { $$ = $1; $$.push_back($3); }
       | ID { $$.push_back($1); }
	   ;

stmt : assignment_stmt
      | input_stmt
      | output_stmt
      | cast_stmt
      | if_stmt
      | while_stmt
      | switch_stmt
      | break_stmt
      | stmt_block
	  ;

assignment_stmt : ID ASSIGN expression SEMICOLON 
{
    auto dest_type = get_var_type_or_error(driver, lexer, $1);
    if (dest_type != Type::Float && $3.type == Type::Float) {
        throw syntax_error(lexer.loc, "cannot implicitly convert float to int");
    }
    auto src_var = cast_if_needed(driver, $3.var, $3.type, dest_type);
    switch (dest_type) {
    case Type::Int:
        driver.gen(Opcode::IASN, $1, src_var);
        break;
    case Type::Float:
        driver.gen(Opcode::RASN, $1, src_var);
        break;
    }
};

input_stmt : INPUT LPAREN ID RPAREN SEMICOLON
{
    auto dest_type = get_var_type_or_error(driver, lexer, $3);
    switch (dest_type) {
    case Type::Int:
        driver.gen(Opcode::IINP, $3);
        break;
    case Type::Float:
        driver.gen(Opcode::RINP, $3);
        break;
    }
};
output_stmt : OUTPUT LPAREN expression RPAREN SEMICOLON
{
    switch ($3.type) {
    case Type::Int:
        driver.gen(Opcode::IPRT, $3.var);
        break;
    case Type::Float:
        driver.gen(Opcode::RPRT, $3.var);
        break;
    }
};

cast_stmt : ID ASSIGN STATIC_CAST LPAREN type RPAREN  LPAREN expression RPAREN SEMICOLON {
        auto dest_type = get_var_type_or_error(driver, lexer, $1);
        // We must cast here as long as type and the expression type differ;
        // even if dest_type is the same as the expression type.
        // This means that we might write 2 casts in this procedure, e.g. if the user
        // static_casts a float to an int and then assigns it to float.
        auto casted_var = cast_if_needed(driver, $8.var, $8.type, $5);
        // The second cast, however, is implicit, and thus must obey implicit casting
        // rules.
        if (dest_type != Type::Float && $5 == Type::Float) {
            throw syntax_error(lexer.loc, "cannot implicitly convert float to int");
        }
        auto src_var = cast_if_needed(driver, casted_var, $5, dest_type);
        switch (dest_type) {
        case Type::Int:
            driver.gen(Opcode::IASN, $1, src_var);
            break;
        case Type::Float:
            driver.gen(Opcode::RASN, $1, src_var);
            break;
        }
    };

if_stmt : IF LPAREN <Label>{
        // Create the false case label
        $$ = Label::make_temp();
    } <Label>{
        // Create the end label
        $$ = Label::make_temp();
    } boolexpr {
        $5->gen(driver, ControlFlow(Fallthrough, $3));
    } RPAREN stmt {
        driver.gen(Opcode::JUMP, $4);
    } ELSE {
        driver.gen_label($3);
    } stmt {
        driver.gen_label($4);
    } ;

while_stmt : WHILE LPAREN <Label>{
        // Create the loop head label and generate it here
        $$ = Label::make_temp();
        driver.gen_label($$);
    }
    <Label>{
        // Create the loop end label
        $$ = Label::make_temp();
    } boolexpr {
        $5->gen(driver, ControlFlow(Fallthrough, $4));
    } RPAREN {
        driver.enter_breakable_scope($4);
    } stmt {
        // Jump to the loop head; also end the loop scope and generate the end label
        driver.gen(Opcode::JUMP, $3);
        driver.gen_label(driver.exit_breakable_scope());
    };

switch_stmt : SWITCH LPAREN expression RPAREN LCURL <Variable>{
        if ($3.type != Type::Int) {
            throw syntax_error(lexer.loc, "cannot switch on an expression of non-integer type");
        }
        // Return the switching variable
        $$ = $3.var;
        driver.enter_breakable_scope(Label::make_temp());
    } caselist DEFAULT COLON stmtlist RCURL {
        // End the switch scope and generate the end label
        driver.gen_label(driver.exit_breakable_scope());
    };
caselist : caselist CASE num COLON <Label>{
        if ($3.type != Type::Int) {
            throw syntax_error(lexer.loc, "cannot switch-case on an expression of non-integer type");
        }
        // Create the next-case label
        $$ = Label::make_temp();
        auto cond = Variable::make_temp();
        driver.gen(Opcode::IEQL, cond, $<Variable>0, $3.var);
        driver.gen(Opcode::JMPZ, $$, cond);
    } stmtlist {
        // Generate the next-case label
        driver.gen_label($5);
    }
         | %empty ;

break_stmt : BREAK SEMICOLON { driver.gen(Opcode::JUMP, driver.get_scope_end()); } ;

stmt_block : LCURL stmtlist RCURL ;

stmtlist  : stmtlist stmt
          | %empty ;

boolexpr : boolexpr OP_OR boolterm { $$ = std::make_unique<BooleanBinaryNode>(std::move($1), std::move($3), BooleanBinaryNode::LogicalOperation::Or); }
         | boolterm { $$ = std::move($1); }
		 ;

boolterm : boolterm OP_AND boolfactor { $$ = std::make_unique<BooleanBinaryNode>(std::move($1), std::move($3), BooleanBinaryNode::LogicalOperation::And); }
         | boolfactor { $$ = std::move($1); }
		 ;

boolfactor : OP_NOT LPAREN boolexpr RPAREN { $$ = std::make_unique<BooleanNotNode>(std::move($3)); }
           | expression  OP_EQ  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::Equal); }
           | expression  OP_NE  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::NotEqual); }
           | expression  OP_LT  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::LessThan); }
           | expression  OP_GT  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::GreaterThan); }
           | expression  OP_LEQ  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::LessEqual); }
           | expression  OP_GEQ  expression { $$ = std::make_unique<BooleanLeafNode>(std::move($1), std::move($3), BooleanLeafNode::CompareOperation::GreaterEqual); }
		   ;

expression : expression OP_ADD term { $$ = gen_arithmetic_op_expr(driver, Opcode::IADD, Opcode::RADD, $1, $3); }
		   | expression OP_SUB term { $$ = gen_arithmetic_op_expr(driver, Opcode::ISUB, Opcode::RSUB, $1, $3); }
           | term { $$ = Expression($1); }
		   ;

term : term OP_MUL factor { $$ = gen_arithmetic_op_expr(driver, Opcode::IMLT, Opcode::RMLT, $1, $3); }
	 | term OP_DIV factor { $$ = gen_arithmetic_op_expr(driver, Opcode::IDIV, Opcode::RDIV, $1, $3); }
     | factor { $$ = Expression($1); }
	 ;

factor : LPAREN expression RPAREN { $$ = Expression($2); }
       | ID { $$ = Expression(get_var_type_or_error(driver, lexer, $1), $1); }
       | num { $$ = Expression($1); }
	   ;

num : NUM_FLOAT { $$ = Expression(Type::Float, Variable::make_temp()); driver.gen(Opcode::RASN, $$.var, $1); }
    | NUM_INT { $$ = Expression(Type::Int, Variable::make_temp()); driver.gen(Opcode::IASN, $$.var, $1); }
    ;
%%
void cpq::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
}