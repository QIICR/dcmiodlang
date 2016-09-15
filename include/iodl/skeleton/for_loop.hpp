#pragma once
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct ForLoop : Statement
        {
            ForLoop( const Status::FilePosition& position );

            virtual Status generate( generator::Context& context );

            std::string          name;
            generator::Component range;
            generator::Component statement;
        };
    }
}
