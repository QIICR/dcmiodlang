#include <cstring>
#include <boost/make_shared.hpp>
#include "iodl/generator/string.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace generator
    {
        CharPointer::CharPointer( const char* str )
        : m_String( str )
        , m_Size( std::strlen( str ) )
        {

        }

        CharPointer::CharPointer( const char* str, const std::size_t size )
        : m_String( str )
        , m_Size( size )
        {

        }

        Status CharPointer::generate( Context& context )
        {
            if( !context.write( m_String, m_Size ) )
                return Status( "error trying to print referenced string value \"" + std::string( m_String, m_Size ) + "\"" );
            return Status();
        }

        StringReference::StringReference( const std::string& str )
        : m_String( str )
        {

        }

        Status StringReference::generate( Context& context )
        {
            if( !context.write( m_String.c_str(), m_String.size() ) )
                return Status( "error trying to print referenced string value \"" + m_String + "\"" );
            return Status();
        }
    }
}
