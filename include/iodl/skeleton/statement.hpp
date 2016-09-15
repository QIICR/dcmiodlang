#pragma once
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Statement : virtual generator::Component::Base
        {
            Statement( const Status::FilePosition& position );
            boost::shared_ptr<Status::FilePosition> position;
        };
    }
}
