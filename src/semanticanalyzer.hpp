#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "ast.hpp"
#include <map>
#include <set>

class SemanticAnalyzer
{
private:
    std::set<std::string> visited;
    std::set<std::string> processed;

    bool invalid_parent(const std::string&); 
    bool cyclic_check(std::map<std::string, std::string>&, const std::string&);

public:
    bool validate_inheritance(const Classes&); 
};


#endif
