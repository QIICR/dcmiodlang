#include <boost/make_shared.hpp>
#include "iodl/skeleton/block.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        Block::Block( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        Status Block::generate( generator::Context& context )
        {
            for
            (
                std::vector<generator::Component>::iterator it = components.begin();
                it != components.end();
                ++it
            )
            {
                Status s = it->generate( context );
                if( !s )
                    return s;
            }
            return Status();
        }
    }
}
