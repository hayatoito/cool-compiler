/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
int num_comment = 0;
int str_too_long = 0;

%}

%x COMMENT
%x LINECOMMENT
%x STRING

/*
 * Define names for regular expressions here.
 */

DARROW          =>

%%

 /*
  *  Nested comments
  */

"(*" {
    BEGIN(COMMENT);
    num_comment++;
}

"*)" {
    if (num_comment <= 0) {
        cool_yylval.error_msg = "Unmatched *)";
        return ERROR;
    }
}

<COMMENT>"*)" {
    num_comment--;
    if (num_comment < 0) {
        cool_yylval.error_msg = "Unmatched *)";
        return ERROR;
    }

    if (num_comment == 0) {
        BEGIN(INITIAL);
    }
}

<COMMENT>"(*" {
    num_comment++;
}

<COMMENT>[^\n] {
    /* eat characters */
}

<COMMENT>\n {
    ++curr_lineno;
}

"--"[^\n]* {
    BEGIN(LINECOMMENT);
}

<LINECOMMENT>\n {
    ++curr_lineno;
    BEGIN(INITIAL);
}

<COMMENT><<EOF>> {
    BEGIN(INITIAL);
    cool_yylval.error_msg = "EOF in comment";
    return ERROR;
}

 /*
  *  The multiple-character operators.
  */
{DARROW}		{ return (DARROW); }

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */

(?i:class) {
    return CLASS;
}

(?i:else) {
    return ELSE;
}

(?i:in) {
    return IN;
}

(?i:then) {
    return THEN;
}

(?i:fi) {
    return FI;
}

(?i:if) {
    return IF;
}

(?i:inherits) {
    return INHERITS;
}

(?i:let) {
    return LET;
}

(?i:loop) {
    return LOOP;
}

(?i:pool) {
    return POOL;
}

(?i:while) {
    return WHILE;
}

(?i:case) {
    return CASE;
}

(?i:esac) {
    return ESAC;
}

(?i:of) {
    return OF;
}

(?i:new) {
    return NEW;
}

(?i:isvoid) {
    return ISVOID;
}

(?i:not) {
    return NOT;
}

t(?i:rue) {
    cool_yylval.boolean = 1;
    return BOOL_CONST;
}

f(?i:alse) {
    cool_yylval.boolean = 0;
    return BOOL_CONST;
}

[0-9]+ {
    cool_yylval.symbol = inttable.add_string(yytext);
    return INT_CONST;
}

"<=" {
    return LE;
}

"<-" {
    return ASSIGN;
}


[A-Z][a-zA-Z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}


[a-z][a-zA-Z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}

";"|","|"{"|"}"|":"|"("|")"|"+"|"-"|"*"|"/"|"="|"~"|"<"|"."|"@" {
    return *yytext;
}

\n {
    ++curr_lineno;
}

[ \f\r\t\v] {
    // eat whitespace
}

 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */

\" {
    BEGIN(STRING);
    string_buf_ptr = string_buf;
    memset(string_buf, 0, MAX_STR_CONST);
}

<STRING>\" {
    BEGIN(INITIAL);
    cool_yylval.symbol = stringtable.add_string(string_buf);
    return STR_CONST;
}

<STRING>\0[^\n]*\" {
    BEGIN(INITIAL);
    if (str_too_long) {
        str_too_long = 0;
    }
    else {
        cool_yylval.error_msg = "String contains null character";
        return ERROR;
    }
}

<STRING>\0[^"]*\n {
    if (str_too_long) {
        yyinput(); /* eat quote */
        BEGIN(INITIAL);
        str_too_long = 0;
    }
    else {
        if (yytext[yyleng - 1] != '\\') {
            BEGIN(INITIAL);
            cool_yylval.error_msg = "String contains null character";
            return ERROR;
        }
    }
}

<STRING><<EOF>> {
    BEGIN(INITIAL);
    cool_yylval.error_msg = "EOF in string constant";
    return ERROR;
}

<STRING>\\ {
    if (strlen(string_buf) >= MAX_STR_CONST - 1) {
        str_too_long = 1;
        unput('\0');
        cool_yylval.error_msg = "String constant too long";
        return ERROR;
    }

    char ahead = yyinput();
    switch (ahead) {
        case 'b':
            *string_buf_ptr++ = '\b';
            break;
        case 't':
            *string_buf_ptr++ = '\t';
            break;
        case 'n':
            *string_buf_ptr++ = '\n';
            break;
        case 'f':
            *string_buf_ptr++ = '\f';
            break;
        case '\n':
            ++curr_lineno;
            *string_buf_ptr++ = '\n';
            break;
        case '\0':
            unput(ahead);
            break;
        default:
            *string_buf_ptr++ = ahead;
    }
}

<STRING>\n {
    ++curr_lineno;
    BEGIN(INITIAL);
    cool_yylval.error_msg = "Unterminated string constant";
    return ERROR;
}

<STRING>. {
    if (strlen(string_buf) >= MAX_STR_CONST - 1) {
        str_too_long = 1;
        unput('\0');
        cool_yylval.error_msg = "String constant too long";
        return ERROR;
    }

    *string_buf_ptr++ = *yytext;
}
    
. /* error for invalid tokens */ {
    cool_yylval.error_msg = yytext;
    return ERROR;
}

%%
