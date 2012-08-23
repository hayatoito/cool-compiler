#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "symboltable.hpp"

// Symbol constants to be used by different stages of the compiler
namespace constants
{
    extern const Symbol OBJECT;
    extern const Symbol INTEGER;
    extern const Symbol BOOLEAN;
    extern const Symbol STRING;
    extern const Symbol IO;
    extern const Symbol SELF;
    extern const Symbol SELF_TYPE;
    extern const Symbol NOCLASS;
    extern const Symbol NOTYPE;
    extern const Symbol PRIM_SLOT;
    extern const Symbol ABORT;
    extern const Symbol TYPE_NAME;
    extern const Symbol COPY;
    extern const Symbol MAIN;
    extern const Symbol OUT_INT;
    extern const Symbol IN_INT;
    extern const Symbol IN_STRING;
    extern const Symbol OUT_STRING;
    extern const Symbol SUBSTR;
    extern const Symbol LENGTH;
    extern const Symbol CONCAT;
    extern const Symbol ARG;
    extern const Symbol ARG2;
    extern const Symbol VAL;
    extern const Symbol STR_FIELD;
}

#endif
