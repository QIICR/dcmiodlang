#pragma once
#include <cstddef>
#include <string>
#include <ostream>
#include <map>
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        struct Context
        {
            virtual ~Context();
            virtual bool put( const int ch ) = 0;
            virtual bool write( const char* data, std::size_t count );
            virtual bool addComponent( const std::string& name, boost::shared_ptr<Component::Base> component ) = 0;
            virtual Component findComponent( const std::string& name ) = 0;
        };

        class StackedContext : public Context
        {
        public:
            StackedContext( Context& context );
            virtual bool put( const int ch );
            virtual bool write( const char* data, std::size_t count );
            virtual bool addComponent( const std::string& name, boost::shared_ptr<Component::Base> component );
            virtual Component findComponent( const std::string& name );

        protected:
            Context& parent;
        };

        class ComponentMapContext : public Context
        {
            typedef std::map<std::string,Component> map_type;

        public:
            ComponentMapContext();
            virtual bool addComponent( const std::string& name, boost::shared_ptr<Component::Base> component );
            virtual Component findComponent( const std::string& name );

        private:
            ComponentMapContext( const ComponentMapContext& );
            ComponentMapContext& operator=( const ComponentMapContext& );

            map_type m_Components;
        };

        class OutputStreamContext : public ComponentMapContext
        {
        public:
            OutputStreamContext( std::ostream& out );
            virtual bool put( int ch );
            virtual bool write( const char* data, std::size_t count );

        private:
            std::ostream& m_OutputStream;
        };

        class StackedOutputStreamContext : public StackedContext
        {
        public:
            StackedOutputStreamContext( Context& context, std::ostream& out );
            virtual bool put( int ch );
            virtual bool write( const char* data, std::size_t count );

        private:
            std::ostream& m_OutputStream;
        };

        class StackedComponentMapContext : public StackedContext
        {
            typedef std::map<std::string,Component> map_type;

        public:
            StackedComponentMapContext( Context& context );
            virtual bool addComponent( const std::string& name, boost::shared_ptr<Component::Base> component );
            virtual Component findComponent( const std::string& name );

        private:
            StackedComponentMapContext( const StackedComponentMapContext& );
            StackedComponentMapContext& operator=( const StackedComponentMapContext& );

            map_type m_Components;
        };
    }
}
