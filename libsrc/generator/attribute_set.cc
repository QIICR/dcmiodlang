#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include "iodl/generator/attribute_set.hpp"
#include "iodl/generator/attribute.hpp"
#include "iodl/generator/string.hpp"

namespace iodl
{
    namespace generator
    {
        struct AttributeSet::Iterator : ComponentIterator::Base
        {
            Iterator( const std::vector<ast::Attribute>::const_iterator& iterator )
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
                return new Attribute( *m_Iterator );
            }

            virtual Base* clone()
            {
                return new Iterator( *this );
            }

            std::vector<ast::Attribute>::const_iterator m_Iterator;
        };

        struct AttributeSet::Flavor : Component::Base
        {
            Flavor( ast::AttributeSet::Flavor flavor )
            : m_Flavor( flavor )
            {

            }

            virtual Status generate( Context& context )
            {
                return Status();
            }

            ast::AttributeSet::Flavor m_Flavor;
        };

        struct AttributeSet::Modifiers : Component::Base
        {
            Modifiers( unsigned modifiers )
            : m_Modifiers( modifiers )
            {

            }

            virtual Status generate( Context& context )
            {
                return Status();
            }

            unsigned m_Modifiers;
        };

        AttributeSet::AttributeSet( const ast::AttributeSet& attributeSet )
        : m_AttributeSet( attributeSet )
        {

        }

        Component AttributeSet::findComponent( const std::string& name )
        {
            static struct LUT
            {
                enum Field
                {
                    UNKNONW,
                    MODIFIERS,
                    FLAVOR,
                    KEYWORD,
                    NAME,
                    SHARED_TYPE
                };

                LUT()
                {
                    table.add( "flavor", FLAVOR );
                    table.add( "modifiers", MODIFIERS );
                    table.add( "keyword", KEYWORD );
                    table.add( "name", NAME );
                    table.add( "sharedType", SHARED_TYPE );
                }

                Field operator[]( const std::string& name )
                {
                    unsigned f;
                    std::string::const_iterator it = name.begin();
                    return boost::spirit::qi::parse( it, name.end(), table, f ) && it == name.end() ? static_cast<Field>( f ) : UNKNONW;
                }

                boost::spirit::qi::symbols<char,unsigned> table;
            } lut;

            switch( lut[name] )
            {
            case LUT::FLAVOR:
                return new Flavor( m_AttributeSet.flavor );
            case LUT::MODIFIERS:
                return new Modifiers( m_AttributeSet.modifiers );
            case LUT::KEYWORD:
                return new StringReference( m_AttributeSet.keyword );
            case LUT::NAME:
                return new StringReference( m_AttributeSet.name );
            case LUT::SHARED_TYPE:
                if( m_AttributeSet.flavor == ast::AttributeSet::FUNCTIONAL_GROUP )
                {
                    if( m_AttributeSet.modifiers & ast::AttributeSet::SHARED )
                    {
                        if( m_AttributeSet.modifiers & ast::AttributeSet::PER_FRAME )
                            return new CharPointer( "EFGS_BOTH" );
                        else
                            return new CharPointer( "EFGS_ONLYSHARED" );
                    }
                    else if( m_AttributeSet.modifiers & ast::AttributeSet::PER_FRAME )
                    {
                        return new CharPointer( "EFGS_ONLYPERFRAME" );
                    }
                    else
                    {
                        return new CharPointer( "EFGS_UNKNOWN" );
                    }
                }
                return Component();
            default:
                return Component();
            }
        }

        ComponentIterator AttributeSet::begin()
        {
            return ComponentIterator( new Iterator( m_AttributeSet.begin() ) );
        }

        ComponentIterator AttributeSet::end()
        {
            return ComponentIterator( new Iterator( m_AttributeSet.end() ) );
        }
    }
}
