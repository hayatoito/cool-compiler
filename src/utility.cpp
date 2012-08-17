#include "utility.hpp"
#include "constants.hpp"

using namespace constants;

namespace utility
{
    bool is_basic_class(const Symbol& class_sym)
    {
        return class_sym == OBJECT || class_sym == IO || class_sym == INTEGER ||
            class_sym == BOOLEAN || class_sym == STRING;
    }

    void print_error(const AstNodePtr& ast, const std::string& msg)
    {
        print_error(ast->filename, ast->line_no, msg);
    }

    void print_error(const std::string& filename, std::size_t line, const std::string& msg)
    {
        std::cerr << filename << ":" << line << ": error: " << msg << "\n";
    }

    void print_error(const std::string& filename, const std::string& msg)
    {
        std::cerr << filename << ": error: " << msg << "\n";
    }

    void print_error(const AstNode& ast, const std::string& msg)
    {
        print_error(ast.filename, ast.line_no, msg);
    }
}
