%{

#include "flexbison.hpp"
#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"

#include <iostream>

#define SETLOC(lval,node) (lval)->setloc((node).first_line, curr_filename)
    
extern std::shared_ptr<Program> ast_root;

extern int yylex();
extern int yylineno;
extern std::string curr_filename;

void yyerror(char *);        
%}
    
%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
%token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
%token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
%token <symbol>  STR_CONST 275 INT_CONST 276 
%token <boolean> BOOL_CONST 277
%token <symbol>  TYPEID 278 OBJECTID 279 
%token ASSIGN 280 NOT 281 LE 282 ERROR 283
    
%type <program> program
%type <clazz> class
%type <classes> class_list
%type <attribute> attribute
%type <attributes> attribute_list
%type <method> method
%type <methods> method_list
%type <expression> expression
%type <expression> let_expr 
%type <expressions> expression_list
%type <expressions> method_expr_list
%type <formal> formal
%type <formals> formal_list
%type <branch> case
%type <cases> case_list
    
/* Precedence declarations go here. */
%nonassoc '='
%left LET
%right ASSIGN
%left NOT
%left '+' '-'
%left '*' '/' 
%left ISVOID
%left '~'
%left '@'
%left '.'
%nonassoc LE '<'

%%
program	: class_list	{ @$ = @1; ast_root = std::make_shared<Program>($1); }
;

class_list : class { $$ = Classes(); $$.push_back($1); }
            | class_list class { $$.push_back($2); }
;

class : CLASS TYPEID '{' attribute_list method_list '}' ';' { $$ = std::make_shared<Class>($2, idtable().add("Object"), $4, $5); SETLOC($$, @1); }
        | CLASS TYPEID INHERITS TYPEID '{' attribute_list method_list '}' ';' { $$ = std::make_shared<Class>($2, $4, $6, $7); SETLOC($$, @1); }
        | error ';' { yyerrok; } 
;

attribute_list : attribute ';' { $$ = Attributes(); $$.push_back($1); }
               | attribute_list attribute ';' { $$.push_back($2); }
               | error ';' { yyerrok; }
;

attribute : OBJECTID ':' TYPEID { $$ = std::make_shared<Attribute>($1, $3, std::make_shared<NoExpr>()); SETLOC($$, @1); }
          | OBJECTID ':' TYPEID ASSIGN expression { $$ = std::make_shared<Attribute>($1, $3, $5); SETLOC($$, @5); }
;

method_list : method ';' { $$ = Methods(); $$.push_back($1); }
            | method_list method ';' { $$.push_back($2); }
            | error ';' { yyerrok; }
;

method : OBJECTID '(' formal_list ')' ':' TYPEID '{' expression '}' { $$ = std::make_shared<Method>($1, $6, $3, $8); SETLOC($$, @1); }
       | OBJECTID '(' ')' ':' TYPEID '{' expression '}' { $$ = std::make_shared<Method>($1, $5, Formals(), $7); SETLOC($$, @1); }
;

formal_list : formal { $$ = Formals(); $$.push_back($1); }
            | formal_list ',' formal { $$.push_back($3); } 
;

formal : OBJECTID ':' TYPEID { $$ = std::make_shared<Formal>($1, $3); SETLOC($$, @1); }
;

case_list : case { $$ = Cases(); $$.push_back($1); }
            | case_list case { $$.push_back($2); }
;

case : OBJECTID ':' TYPEID DARROW expression ';' { $$ = std::make_shared<CaseBranch>($1, $3, $5); SETLOC($$, @5); }
;

method_expr_list : expression { $$ = Expressions(); $$.push_back($1); }
                    | method_expr_list ',' expression { $$.push_back($3); }
;

expression_list : expression ';' { $$ = Expressions(); $$.push_back($1); }
                | expression_list expression ';' { $$.push_back($2); }
                | error ';' { yyerrok; }
;

let_expr : OBJECTID ':' TYPEID IN expression %prec LET { $$ = std::make_shared<Let>($1, $3, std::make_shared<NoExpr>(), $5); SETLOC($$, @5); }
            | OBJECTID ':' TYPEID ASSIGN expression IN expression %prec LET { $$ = std::make_shared<Let>($1, $3, $5, $7); SETLOC($$, @5); }
            | OBJECTID ':' TYPEID ',' let_expr { $$ = std::make_shared<Let>($1, $3, std::make_shared<NoExpr>(), $5); SETLOC($$, @5); }
            | OBJECTID ':' TYPEID ASSIGN expression ',' let_expr { $$ = std::make_shared<Let>($1, $3, $5, $7); SETLOC($$, @4); }
            | error ',' let_expr { yyerrok; }
;
            

expression : OBJECTID ASSIGN expression { $$ = std::make_shared<Assign>($1, $3); SETLOC($$, @3); }
            | expression '.' OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<DynamicDispatch>($1, $3, $5); SETLOC($$, @1); }
            | expression '.' OBJECTID '(' ')' { $$ = std::make_shared<DynamicDispatch>($1, $3, Expressions()); SETLOC($$, @1); }
            | expression '@' TYPEID '.' OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<StaticDispatch>($1, $3, $5, $7); SETLOC($$, @1); }
            | expression '@' TYPEID '.' OBJECTID '(' ')' { $$ = std::make_shared<StaticDispatch>($1, $3, $5, Expressions()); SETLOC($$, @1);}
            | OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<DynamicDispatch>(std::make_shared<Object>(idtable().add("self")), $1, $3); 
                                                  SETLOC($$, @1); } 
            | OBJECTID '(' ')' { $$ = std::make_shared<DynamicDispatch>(std::make_shared<Object>(idtable().add("self")), $1, Expressions()); 
                                 SETLOC($$, @1); } 
            | IF expression THEN expression ELSE expression FI { $$ = std::make_shared<If>($2, $4, $6); SETLOC($$, @2); }
            | WHILE expression LOOP expression POOL { $$ = std::make_shared<While>($2, $4); SETLOC($$, @2); }
            | '{' expression_list '}' { $$ = std::make_shared<Block>($2); SETLOC($$, @2); }
            | LET let_expr { $$ = $2; SETLOC($$, @2); }
            | CASE expression OF case_list ESAC { $$ = std::make_shared<Case>($2, $4); SETLOC($$, @2); }
            | NEW TYPEID { $$ = std::make_shared<New>($2); SETLOC($$, @2); }
            | ISVOID expression { $$ = std::make_shared<IsVoid>($2); SETLOC($$, @2); }
            | expression '+' expression { $$ = std::make_shared<Plus>($1, $3); SETLOC($$, @1); }
            | expression '-' expression { $$ = std::make_shared<Sub>($1, $3); SETLOC($$, @1); }
            | expression '*' expression { $$ = std::make_shared<Mul>($1, $3); SETLOC($$, @1); }
            | expression '/' expression { $$ = std::make_shared<Div>($1, $3); SETLOC($$, @1); }
            | '~' expression { $$ = std::make_shared<Complement>($2); SETLOC($$, @2); }
            | expression '<' expression { $$ = std::make_shared<LessThan>($1, $3); SETLOC($$, @1); }
            | expression LE expression { $$ = std::make_shared<LessThanEqualTo>($1, $3); SETLOC($$, @1); }
            | expression '=' expression { $$ = std::make_shared<EqualTo>($1, $3); SETLOC($$, @1); }
            | NOT expression { $$ = std::make_shared<Not>($2); SETLOC($$, @2); }
            | '(' expression ')' { $$ = $2; SETLOC($$, @2); } 
            | OBJECTID { $$ = std::make_shared<Object>($1); SETLOC($$, @1); }
            | INT_CONST { $$ = std::make_shared<IntConst>($1); SETLOC($$, @1); }
            | STR_CONST { $$ = std::make_shared<StringConst>($1); SETLOC($$, @1); }
            | BOOL_CONST { $$ = std::make_shared<BoolConst>($1); SETLOC($$, @1); } 
;

%%

std::string convert_token(int token)
{
    std::string rep;

    switch (token)
    {
        case CLASS: rep = "class"; break;
        case ELSE: rep = "else"; break;
        case FI: rep = "fi"; break;
        case IF: rep = "if"; break;
        case IN: rep = "in"; break;
        case INHERITS: rep = "inherits"; break;
        case LET: rep = "let"; break;
        case LOOP: rep = "loop"; break;
        case POOL: rep = "pool"; break;
        case THEN: rep = "then"; break;
        case WHILE: rep = "while"; break;
        case CASE: rep = "case"; break;
        case ESAC: rep = "esac"; break;
        case OF: rep = "of"; break;
        case DARROW: rep = "=>"; break;
        case NEW: rep = "new"; break;
        case ISVOID: rep = "isvoid"; break;
        case ASSIGN: rep = "<-"; break;
        case NOT: rep = "not"; break;
        case LE: rep = "<="; break;
        case STR_CONST: rep = "STR_CONST = " + yylval.symbol.get_val(); break;
        case INT_CONST: rep = "INT_CONST = " + yylval.symbol.get_val(); break;
        case BOOL_CONST: rep = "BOOL_CONST = " + yylval.boolean; break;
        case TYPEID: rep = "TYPEID = " + yylval.symbol.get_val(); break;
        case OBJECTID: rep = "OBJECTID = " + yylval.symbol.get_val(); break;
        default: rep = (char) token;
    }     
    
    return rep;
}

void yyerror(char *)
{
    if (yylval.error_msg.length() <= 0)
        std::cerr << curr_filename << ":" << yylineno << ": " << "error: " <<  "syntax error near or at character or token '" << convert_token(yychar) << "'\n";
    else
        std::cerr << curr_filename << ":" << yylineno << ": " << "error: " << yylval.error_msg << "\n";
}


