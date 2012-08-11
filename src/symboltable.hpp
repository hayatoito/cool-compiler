#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <map>
#include <boost/optional.hpp>

// Symbol type used to represent symbols in the language
// including, but not limited to class and method names, and types
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


// Generic symbol table that supports scopes
// Used by the semantic analyzer to enforce scoping rules
// and is used by the code generator to store variable name
// to activation record offset
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

    void add(const K& key, const V& val)
    {
        tbl.back()[key] = val;
    }

    std::size_t size() const
    {
        return tbl.back().size();
    }

    boost::optional<V> probe(const K& key) 
    {
        auto last = tbl.back();
        return last.count(key) > 0 ? boost::optional<V>(last[key]) : boost::optional<V>();
    }

    boost::optional<V> lookup(const K& key)
    {
        for (auto it = tbl.rbegin(), end = tbl.rend(); it != end; ++it)
            if (it->count(key) > 0)
                return boost::optional<V>((*it)[key]);

        return boost::optional<V>();
    }
};

#endif
