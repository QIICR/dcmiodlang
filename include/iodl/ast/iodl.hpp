#pragma once
#include "iodl/ast/attribute_set.hpp"
#include "iodl/ast/iod.hpp"
#include "iodl/ast/files.hpp"

namespace iodl
{
    namespace ast
    {
        typedef boost::variant<IOD,AttributeSet,Files,std::pair<std::string,std::string> > Definition;
        typedef std::vector<Definition> IODL;
    }
}
