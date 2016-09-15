#include <boost/make_shared.hpp>
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        ComponentIterator::Base::~Base()
        {

        }

        void ComponentIterator::Base::increment()
        {

        }

        void ComponentIterator::Base::decrement()
        {

        }

        bool ComponentIterator::Base::equal( Base* rhs )
        {
            return this == rhs;
        }

        Component ComponentIterator::Base::dereference()
        {
            return Component();
        }

        ComponentIterator::Base* ComponentIterator::Base::clone()
        {
            return &ComponentIterator::nil;
        }

        ComponentIterator::Base ComponentIterator::nil;

        ComponentIterator::ComponentIterator( Base* pBase )
        : m_pBase( pBase )
        {

        }

        ComponentIterator::ComponentIterator( const ComponentIterator& rhs )
        : m_pBase( rhs.m_pBase->clone() )
        {

        }

        ComponentIterator& ComponentIterator::operator=( const ComponentIterator& rhs )
        {
            if( this != &rhs )
            {
                clear();
                m_pBase = rhs.m_pBase->clone();
            }
            return *this;
        }

        ComponentIterator::~ComponentIterator()
        {
            clear();
        }

        bool ComponentIterator::operator==( const ComponentIterator& rhs ) const
        {
            return m_pBase->equal( rhs.m_pBase );
        }

        bool ComponentIterator::operator!=( const ComponentIterator& rhs ) const
        {
            return !( (*this) == rhs );
        }

        Component ComponentIterator::operator*() const
        {
            return m_pBase->dereference();
        }

        Component ComponentIterator::operator->() const
        {
            return **this;
        }

        ComponentIterator& ComponentIterator::operator++()
        {
            m_pBase->increment();
            return *this;
        }

        ComponentIterator ComponentIterator::operator++( int )
        {
            ComponentIterator it( *this );
            ++(*this);
            return it.move();
        }

        ComponentIterator& ComponentIterator::operator--()
        {
            m_pBase->decrement();
            return *this;
        }

        ComponentIterator ComponentIterator::operator--( int )
        {
            ComponentIterator it( *this );
            --(*this);
            return it.move();
        }

        void ComponentIterator::clear()
        {
            if( m_pBase != &nil )
                delete m_pBase;
        }

        ComponentIterator::Base* ComponentIterator::move()
        {
            Base* pBase = m_pBase;
            m_pBase = &nil;
            return pBase;
        }

        Component::Base::~Base()
        {

        }

        std::pair<boost::shared_ptr<Component::Base>,Status> Component::Base::evaluate( Context& )
        {
            return std::make_pair( boost::shared_ptr<Base>(), Status() );
        }

        Status Component::Base::generate( Context& context )
        {
            std::pair<boost::shared_ptr<Component::Base>,Status> r = evaluate( context );
            if( !r.second )
                return r.second;
            if( r.first )
                return r.first->generate( context );
            return Status();
        }

        Component Component::Base::findComponent( const std::string& name )
        {
            return Component();
        }

        ComponentIterator Component::Base::begin()
        {
            return ComponentIterator();
        }

        ComponentIterator Component::Base::end()
        {
            return ComponentIterator();
        }

        boost::shared_ptr<Component::Base> Component::Base::clone()
        {
            return Component::nil;
        }

        boost::shared_ptr<Component::Base> Component::nil( new Component::Base );

        Component::Component()
        : m_pBase( nil )
        {

        }

        Component::Component( Base* pBase )
        : m_pBase( pBase )
        {

        }

        Component::Component( const boost::shared_ptr<Base>& rhs )
        : m_pBase( rhs )
        {

        }

        Component::Component( const Component& rhs )
        : m_pBase( rhs.m_pBase )
        {

        }

        Component& Component::operator=( const boost::shared_ptr<Base>& rhs )
        {
            m_pBase = rhs;
            return *this;
        }

        Component& Component::operator=( const Component& rhs )
        {
            m_pBase = rhs.m_pBase;
            return *this;
        }

        Component::operator bool() const
        {
            return m_pBase != nil;
        }

        bool Component::operator!() const
        {
            return m_pBase == nil;
        }

        std::pair<Component,Status> Component::operator()( Context& context )
        {
            std::pair<boost::shared_ptr<Base>,Status> result = m_pBase->evaluate( context );
            return std::make_pair( Component( result.first ? result.first : m_pBase ), result.second );
        }

        Status Component::generate( Context& context ) const
        {
            return m_pBase->generate( context );
        }

        Component Component::findComponent( const std::string& name )
        {
            return m_pBase->findComponent( name );
        }

        ComponentIterator Component::begin()
        {
            return m_pBase->begin();
        }

        ComponentIterator Component::end()
        {
            return m_pBase->end();
        }

        void Component::clear()
        {
            m_pBase.reset();
        }

        boost::shared_ptr<Component::Base> Component::base()
        {
            return m_pBase;
        }
    }
}
