#pragma once
#include <vector>
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        struct Reference : Statement
        {
            Reference( const Status::FilePosition& position );

            virtual std::pair<boost::shared_ptr<generator::Component::Base>,Status>
            evaluate( generator::Context& context );

            std::vector<std::string> identifierPath;
        };
    }
}
