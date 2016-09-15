#pragma once
#include "iodl/skeleton/statement.hpp"
#include "iodl/generator/value_multiplicity.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct ValueMultiplicity : Statement, generator::ValueMultiplicity
        {
            ValueMultiplicity( const Status::FilePosition& position );
        };
    }
}
