#include <boost/make_shared.hpp>
#include "iodl/skeleton/value_multiplicity.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        ValueMultiplicity::ValueMultiplicity( const Status::FilePosition& position )
        : Statement( position )
        , generator::ValueMultiplicity()
        {

        }
    }
}
