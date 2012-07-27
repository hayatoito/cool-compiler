#include "tokentable.hpp"

TokenTable::TokenTable()
    : count(1)
{

}

Symbol TokenTable::add(const std::string& id)
{
   if (tbl.count(id) == 0)
   {
      tbl[id] = Symbol(id);
      idx_tbl[id] = count++;
   }

   return tbl[id];
}

std::map<std::string, Symbol> TokenTable::get_elems() const
{
    return tbl;
}

int TokenTable::get_idx(const std::string& id) 
{
    if (idx_tbl.count(id) > 0)
        return idx_tbl[id];
    else
        return -1;
}
