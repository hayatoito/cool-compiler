#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>

class Symbol 
{
private:
    std::string value;

public:
    Symbol(const std::string& = "");
    std::string get_val() const;
};

inline std::ostream& operator<<(std::ostream& os, const Symbol& sym)
{
    return os << sym.get_val();
}

#endif
