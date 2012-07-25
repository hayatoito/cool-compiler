#include "tokentable.hpp"

Symbol TokenTable::add(const std::string& id)
{
   if (tbl.count(id) == 0)
      tbl[id] = Symbol(id);

   return tbl[id];
}

