#ifndef UTILITY_H
#define UTILITY_H

#include "symboltable.hpp"
#include "ast.hpp"

namespace utility
{
    bool is_basic_class(const Symbol&);
    void print_error(const AstNodePtr&, const std::string&);
    void print_error(const std::string&, std::size_t, const std::string&);
    void print_error(const std::string&, const std::string&);
}

#endif
