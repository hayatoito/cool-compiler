#include "symboltable.hpp"

Symbol::Symbol(const std::string& val)
    : value(val) 
{

}

std::string Symbol::get_val() const 
{
    return value;
}
