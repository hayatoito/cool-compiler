#include "symboltable.hpp"

Symbol::Symbol(const std::string& val)
    : value(val) 
{

}

std::string Symbol::get_val() const 
{
    return value;
}

std::ostream& operator<<(std::ostream& os, const Symbol& sym)
{
    return os << sym.get_val();
}
