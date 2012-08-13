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
}
