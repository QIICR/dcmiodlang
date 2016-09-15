#pragma once
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Text : Statement, std::string
        {
            Text( const Status::FilePosition& position, const std::string& text );
            virtual Status generate( generator::Context& context );
        };
    }
}
