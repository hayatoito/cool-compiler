#include "semanticanalyzer.hpp"
#include "constants.hpp"

#include <iostream>
#include <algorithm>

using namespace constants;

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
    Environment env;
    root->type_check(env);
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
