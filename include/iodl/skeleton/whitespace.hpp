#pragma once
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Whitespace : Statement
        {
            Whitespace( const Status::FilePosition& position );

            virtual Status generate( generator::Context& context );

            generator::Component statement;
        };
    }
}
