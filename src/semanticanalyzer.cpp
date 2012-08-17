#include "semanticanalyzer.hpp"
#include "constants.hpp"
#include "astnodetypechecker.hpp"
#include "utility.hpp"

#include <iostream>
#include <algorithm>

using namespace constants;

void SemanticAnalyzer::install_basic(ProgramPtr& ast_root)
{
    Methods object_methods = { 
        std::make_shared<Method>(ABORT, OBJECT, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(TYPE_NAME, STRING, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(COPY, SELF_TYPE, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(OBJECT, NOCLASS, Attributes(), object_methods));

    Formals io_formal1 = { std::make_shared<Formal>(ARG, STRING) };
    Formals io_formal2 = { std::make_shared<Formal>(ARG, INTEGER) };
    Methods io_methods = {
        std::make_shared<Method>(OUT_STRING, SELF_TYPE, io_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(OUT_INT, SELF_TYPE, io_formal2, std::make_shared<NoExpr>()),
        std::make_shared<Method>(IN_STRING, STRING, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(IN_INT, INTEGER, Formals(), std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(IO, OBJECT, Attributes(), io_methods));

    Attributes int_attributes = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(INTEGER, OBJECT, int_attributes, Methods()));

    Attributes bool_attributes = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>())
    };

    ast_root->classes.push_back(std::make_shared<Class>(BOOLEAN, OBJECT, bool_attributes, Methods()));

    Formals string_formal1 = { std::make_shared<Formal>(ARG, STRING) };
    Formals string_formal2 = { 
        std::make_shared<Formal>(ARG, INTEGER),
        std::make_shared<Formal>(ARG2, INTEGER)
    };

    Methods string_methods = {
        std::make_shared<Method>(LENGTH, INTEGER, Formals(), std::make_shared<NoExpr>()),
        std::make_shared<Method>(CONCAT, STRING, string_formal1, std::make_shared<NoExpr>()),
        std::make_shared<Method>(SUBSTR, STRING, string_formal2, std::make_shared<NoExpr>())
    };

    Attributes string_attributes = {
        std::make_shared<Attribute>(VAL, PRIM_SLOT, std::make_shared<NoExpr>()),
        std::make_shared<Attribute>(STR_FIELD, PRIM_SLOT, std::make_shared<NoExpr>()),    
    };

    ast_root->classes.push_back(std::make_shared<Class>(STRING, OBJECT, string_attributes, string_methods));

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
        utility::print_error(node, "cyclic dependency found in class " + node->name.get_val());
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

        if (invalid_parent(c->parent))
        {
            utility::print_error(c, "cannot inherit from basic class " + c->parent.get_val());
            status = false;
        }

        if (inherit_graph.count(c) > 0)
        {
            if (utility::is_basic_class(c->name))
                utility::print_error(c, "redefinition of basic class " + c->name.get_val() + " not allowed");
            else
                utility::print_error(c, "class " + c->name.get_val() + " has multiple definitions");

            status = false;
        }

        auto parent = std::find_if(begin(classes), end(classes), 
                [&](const ClassPtr& p) { 
                    return c->parent == p->name;
                });

        if (parent == end(classes))
        {
            if (c->name == OBJECT)
            {
                // The Object class' parent is not included in the classes list so we explicitly add a parent NoClass
                // to signal that the end of the class hierarchy is reached.
                inherit_graph[c] = std::make_shared<Class>(NOCLASS, NOCLASS, Attributes(), Methods());
            }
            else
            {
                utility::print_error(c, c->name.get_val() + " inherits from a class that doesn't exist");
                status = false;
            }
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
    AstNodeTypeChecker typechecker(inherit_graph);
    root->accept(typechecker);
    return typechecker.get_err_count() == 0;
}

ClassPtrMap SemanticAnalyzer::get_inherit_graph() const
{
    return inherit_graph;
}
