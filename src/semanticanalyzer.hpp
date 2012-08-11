#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "ast.hpp"

#include <map>
#include <set>
#include <vector>
#include <memory>
#include <functional>

typedef std::map<ClassPtr, ClassPtr> ClassPtrMap;

//Class that contains methods to perform semantic analysis, as well as 
//data structures that are built along the way 
class SemanticAnalyzer
{
private:
    //The following instance variables are used by the cyclic check function
    //to keep track of visited & processed inheritance graph nodes
    std::set<ClassPtr> visited;   
    std::set<ClassPtr> processed;

    ClassPtrMap inherit_graph;

    bool invalid_parent(const Symbol&); 

    //Performs a DFS traversal of the inheritance graph created from the list
    //of classes. It checks for cyclic dependencies between classes in the source code.
    bool cyclic_check(ClassPtrMap&, const ClassPtr&);

    ClassPtr get_parent(const ClassPtr&, const Classes&);

public:
    //Performs a variety of checks to ensure that the class structure, including
    //inheritance is valid 
    bool validate_inheritance(const Classes&); 

    //Calls on the AST to type check and scope check its nodes
    bool type_check(const ProgramPtr&);

    ClassPtrMap get_inherit_graph() const;
    bool is_subtype(const Symbol&, const Symbol&);
};

#endif
