#pragma once
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        struct Comparable
        {
            virtual std::pair<boost::shared_ptr<Component::Base>,Status>
            compare( const Comparable& rhs ) = 0;
        };
    }
}
