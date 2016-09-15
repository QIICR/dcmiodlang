#pragma once
#include <vector>
#include <memory>
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Block : Statement
        {
            Block( const Status::FilePosition& position );

            virtual Status generate( generator::Context& context );

            std::vector<generator::Component> components;
        };
    }
}
