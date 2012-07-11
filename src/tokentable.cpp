#include "tokentable.hpp"

Symbol IdentifierTable::add(const std::string& id)
{
    if (tbl.count(id) == 0)
        tbl[id] = Symbol(id);

    return tbl[id];
}

Symbol IntTable::add(const std::string& num)
{
    if (tbl.count(num) == 0)
        tbl[num] = Symbol(num);

    return tbl[num];
}

Symbol StringTable::add(const std::string& str)
{
    if (tbl.count(str) == 0)
        tbl[str] = Symbol(str);

    return tbl[str];
}
