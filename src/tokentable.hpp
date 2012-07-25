#ifndef TOKENTABLE_H
#define TOKENTABLE_H

#include "symboltable.hpp"
#include <string>
#include <map>

class TokenTable
{
private:
    std::map<std::string, Symbol> tbl;

public:
    Symbol add(const std::string&);

};

class IdentifierTable : public TokenTable
{

};

inline IdentifierTable& idtable()
{
    static IdentifierTable idtable;
    return idtable;
}

class IntTable : public TokenTable
{

};

inline IntTable& inttable()
{
    static IntTable inttable;
    return inttable;
}

class StringTable : public TokenTable
{

};

inline StringTable& stringtable()
{
    static StringTable stringtable;
    return stringtable;
}

#endif 
