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

inline bool operator==(const Symbol& s1, const Symbol& s2)
{
    return s1.get_val() == s2.get_val();
}

inline bool operator!=(const Symbol& s1, const Symbol& s2)
{
    return !(s1.get_val() == s2.get_val());
}

#endif
