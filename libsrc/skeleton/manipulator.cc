#include <cctype>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include "iodl/skeleton/whitespace.hpp"
#include "iodl/skeleton/toupper.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        Whitespace::Whitespace( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        Status Whitespace::generate( generator::Context& context )
        {
            struct Context : generator::StackedContext
            {
                inline Context( generator::Context& parent )
                : generator::StackedContext( parent )
                {

                }

                inline virtual bool put( const int ch )
                {
                    return generator::StackedContext::put( ' ' );
                }

                inline virtual bool write( const char* data, const std::size_t count )
                {
                    for( std::size_t n = 0; n < count; ++n )
                        if( !generator::StackedContext::put( ' ' ) )
                            return false;
                    return true;
                }
            } whitespace( context );
            return statement.generate( whitespace );
        }

        Toupper::Toupper( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        Status Toupper::generate( generator::Context& context )
        {
            struct Context : generator::StackedContext
            {
                inline Context( generator::Context& parent )
                : generator::StackedContext( parent )
                {

                }

                inline virtual bool put( const int ch )
                {
                    return generator::StackedContext::put( std::toupper( ch ) );
                }

                inline virtual bool write( const char* data, const std::size_t count )
                {
                    std::string upper( data, count );
                    boost::to_upper( upper );
                    return generator::StackedContext::write( upper.c_str(), upper.size() );
                }
            } toupper( context );
            return statement.generate( toupper );
        }
    }
}
