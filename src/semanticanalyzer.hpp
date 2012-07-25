#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "ast.hpp"
#include <map>
#include <set>
#include <vector>
#include <memory>

class Program;
class Class;
typedef std::vector<std::shared_ptr<Class>> Classes;

//Environment used for bulk of semantic analysis - type check & scope
struct Environment
{
    SymbolTable<Symbol, Symbol> symtbl; //Table that keep tracks of variables, their scopes, and 
                                        //and their types. Key = variable name, Value = type
                                        
    Symbol curr_class; //Keeps track of the enclosing class while type checking expressions.
                       //This is used to handle SELF_TYPE

};

//Class that contains methods to perform semantic analysis, as well as 
//data structures that are built along the way 
class SemanticAnalyzer
{
private:
    //The following instance variables are used by the cyclic check function
    //to keep track of visited & processed inheritance graph nodes
    std::set<std::string> visited;   
    std::set<std::string> processed;

    std::map<std::string, std::string> inherit_graph;

    bool invalid_parent(const std::string&); 

    //Performs a DFS traversal of the inheritance graph created from the list
    //of classes. It checks for cyclic dependencies between classes in the source code.
    bool cyclic_check(std::map<std::string, std::string>&, const std::string&);

public:
    //Performs a variety of checks to ensure that the class structure, including
    //inheritance is valid 
    bool validate_inheritance(const Classes&); 

    //Calls on the AST to type check and scope check its nodes
    bool type_check(const std::shared_ptr<Program>&);

    std::map<std::string, std::string> get_inherit_graph() const;
};


#endif
