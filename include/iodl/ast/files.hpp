#pragma once
#include <map>
#include <string>

namespace iodl
{
    namespace ast
    {
        struct Files : std::map<std::string,std::string>
        {
            std::string keyword;
        };
    }
}
