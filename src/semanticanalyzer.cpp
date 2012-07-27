#include "semanticanalyzer.hpp"
#include <iostream>

bool SemanticAnalyzer::invalid_parent(const std::string& parent)
{
    return parent == "String" || parent == "Bool" || parent == "Int"
        || parent == "SELF_TYPE";
}

bool SemanticAnalyzer::cyclic_check(std::map<std::string, std::string>& graph, const std::string& node)
{
    if (node == "Object")
        return true;

    if (visited.count(node) == 1 && processed.count(node) == 0)
    {
        std::cerr << "Cyclic dependency found in class " << node << "\n";
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
    for (auto& c : classes)
    {
        std::string class_name = c->name.get_val();

        if (invalid_parent(class_name) || class_name == "Object" || class_name == "IO")
        {
            std::cerr << "Redefinition of a basic class not allowed.\n";
            return false;
        }

        if (invalid_parent(c->parent.get_val()))
        {
            std::cerr << "Cannot inherit from any of the basic classes - IO, String, Bool, Int\n";
            return false;
        }


        if (inherit_graph.count(class_name) > 0)
        {
            std::cerr << "Class " << class_name << " has multiple definitions.\n";
            return false;
        }

        inherit_graph[class_name] = c->parent.get_val(); 
    }

    for (auto it = begin(inherit_graph); it != end(inherit_graph); ++it)
        if (!cyclic_check(inherit_graph, it->first))
            return false;

    return true;
}

bool SemanticAnalyzer::type_check(const std::shared_ptr<Program>& root)
{
    Environment env;
    root->type_check(*this, env);
}

std::map<std::string, std::string> SemanticAnalyzer::get_inherit_graph() const
{
    return inherit_graph;
}
