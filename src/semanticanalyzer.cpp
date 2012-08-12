#include "semanticanalyzer.hpp"
#include "constants.hpp"
#include "astnodetypechecker.hpp"

#include <iostream>
#include <algorithm>

using namespace constants;

void SemanticAnalyzer::install_basic(ProgramPtr& ast_root)
{
    Features object_features = { 
        std::make_shared<Method>(ABORT, OBJECT, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(TYPE_NAME, STRING, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(COPY, SELF_TYPE, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(OBJECT, NOCLASS, idtable().add("filename"), object_features));

    Formals io_formal1 = { std::make_shared<Formal>(ARG, STRING) };
    Formals io_formal2 = { std::make_shared<Formal>(ARG, INTEGER) };
    Features io_features = {
        std::make_shared<Method>(OUT_STRING, SELF_TYPE, io_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(OUT_INT, SELF_TYPE, io_formal2, std::make_shared<NoExpr>()),
        std::make_shared<Method>(IN_STRING, STRING, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(IN_INT, INTEGER, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(IO, OBJECT, idtable().add("filename"), io_features));

    Features int_features = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(INTEGER, OBJECT, idtable().add("filename"), int_features));

    Features bool_features = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(BOOLEAN, OBJECT, idtable().add("filename"), bool_features));

    Formals string_formal1 = { std::make_shared<Formal>(ARG, STRING) };
    Formals string_formal2 = { 
        std::make_shared<Formal>(ARG, INTEGER),
        std::make_shared<Formal>(ARG2, INTEGER)
    };

    Features string_features = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>()),
        std::make_shared<Attribute>(STR_FIELD, PRIM_SLOT, std::make_shared<NoExpr>()),    
        std::make_shared<Method>(LENGTH, INTEGER, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(CONCAT, STRING, string_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(SUBSTR, STRING, string_formal2, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(STRING, OBJECT, idtable().add("filename"), string_features));

    //Add basic classes to string table so a string constant (codegen)
    //will be created for them
    stringtable().add("Object");
    stringtable().add("Bool");
    stringtable().add("IO");
    stringtable().add("Int");
    stringtable().add("String");
}

bool SemanticAnalyzer::invalid_parent(const Symbol& parent)
{
    return parent == STRING || parent == BOOLEAN || parent == INTEGER;
}

bool SemanticAnalyzer::cyclic_check(ClassPtrMap& graph, const ClassPtr& node)
{
    if (node->name == OBJECT || node->name == IO)
        return true;

    if (visited.count(node) == 1 && processed.count(node) == 0)
    {
        std::cerr << "Cyclic dependency found in class " << node->name << "\n";
        return false;
    }

    visited.insert(node);

    if(!cyclic_check(graph, graph[node]))
        return false;

    processed.insert(node);
    return true;
}

bool SemanticAnalyzer::validate_inheritance(const Classes& classes)
{
    bool status = true;

    for (auto& c : classes)
    {
        if (invalid_parent(c->name) || c->name == OBJECT || c->name == IO)
        {
            std::cerr << "error:Redefinition of basic class " << c->name << " not allowed.\n";
            status = false;
        }

        if (invalid_parent(c->parent))
        {
            std::cerr << "error:Cannot inherit from any of the basic classes - IO, String, Bool, Int\n";
            status = false;
        }

        if (inherit_graph.count(c) > 0)
        {
            std::cerr << "error:Class " << c->name << " has multiple definitions.\n";
            status = false;
        }

        auto parent = std::find_if(begin(classes), end(classes), 
                [&](const ClassPtr& p) { 
                    return c->parent == p->name;
                });

        if (parent == end(classes))
        {
            std::cerr << "error:" << c->name << " inherits from a class that doesn't exist.\n";
            status = false;
        }
        else
        {
            inherit_graph[c] = *parent;
        }
    }

    auto main_class = std::find_if(begin(classes), end(classes),
                [&](const ClassPtr& p) {
                    return p->name == MAIN;
                }); 

    if (main_class == end(classes))
    {
        std::cerr << "error:Main class not found.\n";
        status = false;
    }

    for (auto& elem : inherit_graph)
        if (!cyclic_check(inherit_graph, elem.first))
            status = false;

    return status;
}

bool SemanticAnalyzer::type_check(const ProgramPtr& root)
{
    AstNodeTypeChecker typechecker;
    root->accept(typechecker);
}

bool SemanticAnalyzer::is_subtype(const Symbol& child, const Symbol& parent)
{
    /*
    if (schild == sparent) return true;

    Symbol curr(inherit_graph[schild]
    while(curr != "Object") 
    {
        if (curr == sparent)
            return true;

        curr = adjacencylist_[curr].parent;
    }

    return false;
    */
}

ClassPtrMap SemanticAnalyzer::get_inherit_graph() const
{
    return inherit_graph;
}
