#include "constants.hpp"
#include "tokentable.hpp"

namespace constants
{
    const Symbol OBJECT = idtable().add("Object");
    const Symbol INTEGER = idtable().add("Int");
    const Symbol BOOLEAN = idtable().add("Bool");
    const Symbol STRING = idtable().add("String");
    const Symbol MAIN = idtable().add("Main");
    const Symbol IO = idtable().add("IO");
    const Symbol SELF = idtable().add("self");
    const Symbol SELF_TYPE = idtable().add("SELF_TYPE");
    const Symbol NOCLASS = idtable().add("NoClass");
    const Symbol NOTYPE = idtable().add("NoType");
    const Symbol PRIM_SLOT = idtable().add("prim_slot");
    const Symbol ABORT = idtable().add("abort");
    const Symbol TYPE_NAME = idtable().add("type_name");
    const Symbol COPY = idtable().add("copy");
    const Symbol OUT_INT = idtable().add("out_int");
    const Symbol IN_INT = idtable().add("in_int");
    const Symbol IN_STRING = idtable().add("in_string");
    const Symbol OUT_STRING = idtable().add("out_string");
    const Symbol SUBSTR = idtable().add("substr");
    const Symbol LENGTH = idtable().add("length");
    const Symbol CONCAT = idtable().add("concat");
    const Symbol ARG = idtable().add("arg");
    const Symbol ARG2 = idtable().add("arg2");
    const Symbol VAL = idtable().add("val");
    const Symbol STR_FIELD = idtable().add("str_field");
}
