#pragma once
#include <boost/make_shared.hpp>
#include "iodl/generator/expression.hpp"

namespace iodl
{
    namespace generator
    {
        template<typename T,typename Adapter>
        class Container : public Boolean
        {
            struct Iterator : ComponentIterator::Base
            {
                Iterator( const typename T::const_iterator& iterator )
                : m_Iterator( iterator )
                {

                }

                virtual void increment()
                {
                    ++m_Iterator;
                }

                virtual void decrement()
                {
                    --m_Iterator;
                }

                virtual bool equal( Base* rhs )
                {
                    Iterator* it = dynamic_cast<Iterator*>( rhs );
                    return it && it->m_Iterator == m_Iterator;
                }

                virtual Component dereference()
                {
                    return new Adapter( *m_Iterator );
                }

                virtual Base* clone()
                {
                    return new Iterator( *this );
                }

                typename T::const_iterator m_Iterator;
            };

        public:
            Container( const T& t )
            : m_Container( t )
            {

            }

            virtual ComponentIterator begin()
            {
                return ComponentIterator( new Iterator( m_Container.begin() ) );
            }

            virtual ComponentIterator end()
            {
                return ComponentIterator( new Iterator( m_Container.end() ) );
            }

            virtual bool result() const
            {
                return !m_Container.empty();
            }

        protected:
            const T& m_Container;
        };

        template<typename Adapter,typename T>
        boost::shared_ptr<Container<T,Adapter> > make_container( const T& t )
        {
            return boost::make_shared<Container<T,Adapter> >( t );
        }
    }
}
