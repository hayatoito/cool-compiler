#ifndef TOKENTABLE_H
#define TOKENTABLE_H

#include "symboltable.hpp"
#include <string>
#include <map>

class IdentifierTable 
{
private:
    std::map<std::string, Symbol> tbl;

public:
    Symbol add(const std::string&);
};


class IntTable
{
private:
    std::map<std::string, Symbol> tbl;

public:
    Symbol add(const std::string&);
};


class StringTable
{
private:
    std::map<std::string, Symbol> tbl;

public:
    Symbol add(const std::string&);
};

#endif 
