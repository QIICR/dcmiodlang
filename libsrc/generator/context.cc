#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace generator
    {
        Context::~Context()
        {

        }

        bool Context::write( const char* data, std::size_t count )
        {
            for( std::size_t n = 0; n < count; ++n )
                if( !put( *data++ ) )
                    return false;
            return true;
        }

        StackedContext::StackedContext( Context& context )
        : parent( context )
        {

        }

        bool StackedContext::put( const int ch )
        {
            return parent.put( ch );
        }

        bool StackedContext::write( const char* data, std::size_t count )
        {
            return parent.write( data, count );
        }

        bool StackedContext::addComponent( const std::string& name,
                                           boost::shared_ptr<Component::Base> component )
        {
            return parent.addComponent( name, component );
        }

        Component StackedContext::findComponent( const std::string& name )
        {
            return parent.findComponent( name );
        }

        ComponentMapContext::ComponentMapContext()
        {

        }

        bool ComponentMapContext::addComponent( const std::string& name,
                                                boost::shared_ptr<Component::Base> component )
        {
            return m_Components.insert( std::make_pair( name, component ) ).second;
        }

        Component ComponentMapContext::findComponent( const std::string& name )
        {
            map_type::iterator it = m_Components.find( name );
            if( it != m_Components.end() )
                return it->second;
            return Component();
        }

        OutputStreamContext::OutputStreamContext( std::ostream& out )
        : m_OutputStream( out )
        {

        }

        bool OutputStreamContext::put( int ch )
        {
            return m_OutputStream.put( ch ).good();
        }

        bool OutputStreamContext::write( const char* data, std::size_t count )
        {
            return m_OutputStream.write( data, count ).good();
        }


        StackedOutputStreamContext::StackedOutputStreamContext( Context& context, std::ostream& out )
        : StackedContext( context )
        , m_OutputStream( out )
        {

        }

        bool StackedOutputStreamContext::put( int ch )
        {
            return m_OutputStream.put( ch ).good();
        }

        bool StackedOutputStreamContext::write( const char* data, std::size_t count )
        {
            return m_OutputStream.write( data, count ).good();
        }

        StackedComponentMapContext::StackedComponentMapContext( Context& context )
        : StackedContext( context )
        {

        }

        bool StackedComponentMapContext::addComponent( const std::string& name,
                                                       boost::shared_ptr<Component::Base> component )
        {
            return m_Components.insert( std::make_pair( name, component ) ).second;
        }

        Component StackedComponentMapContext::findComponent( const std::string& name )
        {
            map_type::iterator it = m_Components.find( name );
            if( it != m_Components.end() )
                return it->second;
            return StackedContext::findComponent( name );
        }
    }
}
