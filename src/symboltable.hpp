#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <map>

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

inline bool operator<(const Symbol& s1, const Symbol& s2)
{
    return s1.get_val() < s2.get_val();
}


template<typename K, typename V>
class SymbolTable
{
private:
    typename std::vector<std::map<K, V>> tbl;

public:
    void enter_scope()
    {
        tbl.push_back(std::map<K, V>());
    }

    void exit_scope()
    {
        tbl.pop_back();
    }

    void add(K key, V val)
    {
        tbl.back()[key] = val;
    }

    V probe(K key)
    {
        auto last = tbl.back();
        if (last.count(key) > 0)
            return last[key];
        else
            throw("key does not exist");
    }

    V lookup(K key)
    {
        for (auto it = tbl.rbegin(); it != tbl.rend(); ++it)
            if (it->count(key) > 0)
                return (*it)[key];

        throw("key does not exist");
    }
};

#endif
