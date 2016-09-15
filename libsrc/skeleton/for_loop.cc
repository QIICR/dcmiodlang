#include <boost/make_shared.hpp>
#include "iodl/skeleton/for_loop.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        ForLoop::ForLoop( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        Status ForLoop::generate( generator::Context& context )
        {
            struct Context : generator::StackedContext
            {
                inline Context( generator::Context& parent,
                                const std::string& name,
                                const generator::ComponentIterator& it )
                : generator::StackedContext( parent )
                , name( name )
                , it( it )
                {

                }

                virtual generator::Component findComponent( const std::string& name )
                {
                    if( name == Context::name )
                        return *it;
                    return generator::StackedContext::findComponent( name );
                }

                const std::string& name;
                generator::ComponentIterator it;
            };

            std::pair<generator::Component,Status> r = range( context );

            if( !r.second )
                return r.second;

            for( Context c( context, name, r.first.begin() ); c.it != r.first.end(); ++c.it )
            {
                Status s = statement.generate( c );
                if( !s )
                    return s;
            }

            return Status();
        }
    }
}
