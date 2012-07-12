%{

#define YY_DECL int yylex (semantic_type* yylval, location_type* yylloc)
YY_DECL;;

#include "flexbison.hpp"
#include "symboltable.hpp"
#include "tokentable.hpp"
#include "ast.hpp"

#include <iostream>
    
void yyerror(char *s);        

extern std::shared_ptr<Program> ast_root;
extern IdentifierTable idtable;
extern IntTable inttable;
extern StringTable stringtable;

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
%type <feature> feature
%type <features> feature_list
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

class_list : class { $$ = std::vector<std::shared_ptr<Class>>(); $$.push_back($1); }
            | class_list class { $1.push_back($2); }
;

class : CLASS TYPEID '{' feature_list '}' ';' { $$ = std::make_shared<Class>($2, idtable.add("Object"), stringtable.add("filename"), $4); }
        | CLASS TYPEID INHERITS TYPEID '{' feature_list '}' ';' { $$ = std::make_shared<Class>($2, $4, stringtable.add("filename"), $6); }
        | error ';' { } 
;

/* Feature list may be empty, but no empty features in list. */
feature_list : /* no methods */ { $$ = std::vector<std::shared_ptr<Feature>>(); }
                | feature_list feature { $1.push_back($2); }
;

feature : OBJECTID '(' formal_list ')' ':' TYPEID '{' expression '}' ';' { $$ = std::make_shared<Method>($1, $6, $3, $8); }
        | OBJECTID '(' ')' ':' TYPEID '{' expression '}' ';' { $$ = std::make_shared<Method>($1, $5, Formals(), $7); }
        | OBJECTID ':' TYPEID ';' { $$ = std::make_shared<Attribute>($1, $3, std::make_shared<NoExpr>()); }
        | OBJECTID ':' TYPEID ASSIGN expression ';' { $$ = std::make_shared<Attribute>($1, $3, $5); }
        | error ';' { } 
;

formal_list : formal { $$ = std::vector<std::shared_ptr<Formal>>(); $$.push_back($1); }
            | formal_list ',' formal { $1.push_back($3); } 
;

formal : OBJECTID ':' TYPEID { $$ = std::make_shared<Formal>($1, $3); }
;

case_list : case { $$ = std::vector<std::shared_ptr<CaseBranch>>(); $$.push_back($1); }
            | case_list case { $1.push_back($2); }
;

case : OBJECTID ':' TYPEID DARROW expression ';' { $$ = std::make_shared<CaseBranch>($1, $3, $5); }
;

method_expr_list : expression { $$ = std::vector<std::shared_ptr<Expression>>(); $$.push_back($1); }
                    | method_expr_list ',' expression { $1.push_back($3); }
;

expression_list : expression ';' { $$ = std::vector<std::shared_ptr<Expression>>(); $$.push_back($1); }
                | expression_list expression ';' { $1.push_back($2); }
                | error ';' { }
;

let_expr : OBJECTID ':' TYPEID IN expression %prec LET { $$ = std::make_shared<Let>($1, $3, std::make_shared<NoExpr>(), $5); }
            | OBJECTID ':' TYPEID ASSIGN expression IN expression %prec LET { $$ = std::make_shared<Let>($1, $3, $5, $7); }
            | OBJECTID ':' TYPEID ',' let_expr { $$ = std::make_shared<Let>($1, $3, std::make_shared<NoExpr>(), $5); }
            | OBJECTID ':' TYPEID ASSIGN expression ',' let_expr { $$ = std::make_shared<Let>($1, $3, $5, $7); }
            | error ',' let_expr { }
;
            

expression : OBJECTID ASSIGN expression { $$ = std::make_shared<Assign>($1, $3); }
            | expression '.' OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<DynamicDispatch>($1, $3, $5); }
            | expression '.' OBJECTID '(' ')' { $$ = std::make_shared<DynamicDispatch>($1, $3, Expressions()); }
            | expression '@' TYPEID '.' OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<StaticDispatch>($1, $3, $5, $7); }
            | expression '@' TYPEID '.' OBJECTID '(' ')' { $$ = std::make_shared<StaticDispatch>($1, $3, $5, Expressions()); }
            | OBJECTID '(' method_expr_list ')' { $$ = std::make_shared<DynamicDispatch>(std::make_shared<Object>(idtable.add("self")), $1, $3); } 
            | OBJECTID '(' ')' { $$ = std::make_shared<DynamicDispatch>(std::make_shared<Object>(idtable.add("self")), $1, Expressions()); } 
            | IF expression THEN expression ELSE expression FI { $$ = std::make_shared<If>($2, $4, $6); }
            | WHILE expression LOOP expression POOL { $$ = std::make_shared<While>($2, $4); }
            | '{' expression_list '}' { $$ = std::make_shared<Block>($2); }
            | LET let_expr { $$ = $2; }
            | CASE expression OF case_list ESAC { $$ = std::make_shared<Case>($2, $4); }
            | NEW TYPEID { $$ = std::make_shared<New>($2); }
            | ISVOID expression { $$ = std::make_shared<IsVoid>($2); }
            | expression '+' expression { $$ = std::make_shared<Plus>($1, $3); }
            | expression '-' expression { $$ = std::make_shared<Sub>($1, $3); }
            | expression '*' expression { $$ = std::make_shared<Mul>($1, $3); }
            | expression '/' expression { $$ = std::make_shared<Div>($1, $3); }
            | '~' expression { $$ = std::make_shared<Complement>($2); }
            | expression '<' expression { $$ = std::make_shared<LessThan>($1, $3); }
            | expression LE expression { $$ = std::make_shared<LessThanEqualTo>($1, $3); }
            | expression '=' expression { $$ = std::make_shared<EqualTo>($1, $3); }
            | NOT expression { $$ = std::make_shared<Not>($2); }
            | '(' expression ')' { $$ = $2; } 
            | OBJECTID { $$ = std::make_shared<Object>($1); }
            | INT_CONST { $$ = std::make_shared<IntConst>($1); }
            | STR_CONST { $$ = std::make_shared<StringConst>($1); }
            /* | BOOL_CONST { $$ = std::make_shared<BoolConst>($1); } */
;

%%

void yyerror(char *s)
{
    std::cerr << "Error!\n";
}


