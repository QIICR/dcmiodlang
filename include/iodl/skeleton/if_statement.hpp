#pragma once
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct IfStatement : Statement
        {
            IfStatement( const Status::FilePosition& positon );

            virtual Status generate( generator::Context& context );

            generator::Component condition;
            generator::Component then_statement;
            generator::Component else_statement;
        };
    }
}
