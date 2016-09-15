#pragma once
#include "iodl/status.hpp"

namespace iodl
{
    namespace generator
    {
        struct Context;
        class Component;

        class ComponentIterator
        {
        public:
            struct Base;

            ComponentIterator( Base* pBase = &nil );
            ComponentIterator( const ComponentIterator& rhs );
            ComponentIterator& operator=( const ComponentIterator& rhs );
            ~ComponentIterator();
            bool operator==( const ComponentIterator& rhs ) const;
            bool operator!=( const ComponentIterator& rhs ) const;
            Component operator*() const;
            Component operator->() const;
            ComponentIterator& operator++();
            ComponentIterator operator++( int );
            ComponentIterator& operator--();
            ComponentIterator operator--( int );

        private:
            static Base nil;

            void clear();
            Base* move();

            Base* m_pBase;
        };

        class Component
        {
        public:
            struct Base
            {
                virtual ~Base();
                virtual std::pair<boost::shared_ptr<Base>,Status> evaluate( Context& context );
                virtual Status generate( Context& context );
                virtual Component findComponent( const std::string& name );
                virtual ComponentIterator begin();
                virtual ComponentIterator end();
                virtual boost::shared_ptr<Base> clone();
            };

            template<typename T>
            T* as() const
            {
                return dynamic_cast<T*>( m_pBase.get() );
            }

            Component();
            Component( Base* pBase );
            Component( const boost::shared_ptr<Base>& pBase );
            Component( const Component& rhs );
            Component& operator=( const boost::shared_ptr<Base>& rhs );
            Component& operator=( const Component& rhs );
            operator bool() const;
            bool operator!() const;
            std::pair<Component,Status> operator()( Context& context );
            Status generate( Context& ) const;
            Component findComponent( const std::string& name );
            ComponentIterator begin();
            ComponentIterator end();
            boost::shared_ptr<Base> base();

        private:
            static boost::shared_ptr<Base> nil;

            void clear();

            boost::shared_ptr<Base> m_pBase;
        };

        struct ComponentIterator::Base
        {
            virtual ~Base();
            virtual void increment();
            virtual void decrement();
            virtual bool equal( Base* rhs );
            virtual Component dereference();
            virtual Base* clone();
        };
    }
}
