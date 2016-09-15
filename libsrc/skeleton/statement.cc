#include <boost/make_shared.hpp>
#include "iodl/skeleton/statement.hpp"

namespace iodl
{
    namespace skeleton
    {
        Statement::Statement( const Status::FilePosition& position )
        : position( new Status::FilePosition( position ) )
        {

        }
    }
}
