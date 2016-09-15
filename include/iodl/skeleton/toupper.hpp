#pragma once
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Toupper : Statement
        {
            Toupper( const Status::FilePosition& position );

            virtual Status generate( generator::Context& context );

            generator::Component statement;
        };
    }
}
