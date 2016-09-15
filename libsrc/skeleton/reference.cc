#include <boost/make_shared.hpp>
#include "iodl/skeleton/reference.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        Reference::Reference( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        std::pair<boost::shared_ptr<generator::Component::Base>,Status>
        Reference::evaluate( generator::Context& context )
        {
            std::vector<std::string>::const_iterator it = identifierPath.begin();
            if( it != identifierPath.end() )
            {
                generator::Component destination = context.findComponent( *it );
                while( destination && ++it != identifierPath.end() )
                    destination = destination.findComponent( *it );
                if( !destination )
                {
                    std::vector<std::string>::const_iterator p = identifierPath.begin();
                    if( p != it )
                    {
                        std::string parent = *p;
                        while( ++p != it )
                            parent += ( '.' + *p );
                        return std::make_pair
                        (
                            boost::shared_ptr<generator::Component::Base>(),
                            Status
                            (
                                "no field named \"" + *it + "\" in context \"" + parent + '"',
                                position
                            )
                        );
                    }
                    return std::make_pair
                    (
                        boost::shared_ptr<generator::Component::Base>(),
                        Status( "unknown property \"" + *it + '"', position )
                    );
                }
                return std::make_pair( destination.base(), Status() );
            }
            return std::make_pair
            (
                boost::shared_ptr<generator::Component::Base>(),
                Status( "reference node without content", position )
            );
        }
    }
}
