#include <boost/make_shared.hpp>
#include "iodl/skeleton/text.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        Text::Text(  const Status::FilePosition& position, const std::string& text )
        : Statement( position )
        , std::string( text )
        {

        }

        Status Text::generate( generator::Context& context )
        {
            if( !context.write( Text::c_str(), Text::size() ) )
                return Status( "could not write text \"" + (*this) + "\" to output", position );
            return Status();
        }
    }
}
