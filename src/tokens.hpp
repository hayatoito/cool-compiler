#ifndef TOKENS_HPP
#define TOKENS_HPP

#include "symboltable.hpp"

#ifndef YYTOKENTYPE
#define YYTOKENTYPE
    enum yytokentype 
    {
        CLASS = 258,
        ELSE = 259,
        FI = 260,
        IF = 261,
        IN = 262,
        INHERITS = 263,
        LET = 264,
        LOOP = 265,
        POOL = 266,
        THEN = 267,
        WHILE = 268,
        CASE = 269,
        ESAC = 270,
        OF = 271,
        DARROW = 272,
        NEW = 273,
        ISVOID = 274,
        STR_CONST = 275,
        INT_CONST = 276,
        BOOL_CONST = 277,
        TYPEID = 278,
        OBJECTID = 279,
        ASSIGN = 280,
        NOT = 281,
        LE = 282,
        ERROR = 283,
        LET_STMT = 285
    };
#endif

union YYSTYPE
{
    const char* error_msg;
    bool boolean;
    const char* int_const;
    const char* str_const;
    const char* id;
};

#endif
