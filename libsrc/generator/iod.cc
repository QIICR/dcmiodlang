#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include "iodl/generator/iod.hpp"
#include "iodl/generator/container.hpp"
#include "iodl/generator/string.hpp"

namespace iodl
{
    namespace generator
    {
        struct IOD::ModuleReference : StringReference
        {
            ModuleReference( const ast::IOD::ModuleReference& module )
            : StringReference( module )
            {

            }

            Component findComponent( const std::string& name )
            {
                if( name == "usage" ) switch( static_cast<const ast::IOD::ModuleReference&>( m_String ).usage )
                {
                case ast::IOD::MANDATORY:
                    return new CharPointer( "M" );
                case ast::IOD::CONDITIONAL:
                    return new CharPointer( "C" );
                case ast::IOD::USER_OPTION:
                    return new CharPointer( "U" );
                }

                return Component();
            }
        };

        struct IOD::FunctionalGroupReference : StringReference
        {
            FunctionalGroupReference( const ast::IOD::FunctionalGroupReference& fg )
            : StringReference( fg )
            {

            }

            Component findComponent( const std::string& name )
            {
                static struct LUT
                {
                    enum Field
                    {
                        UNKNONW,
                        USAGE,
                        SHARED_TYPE
                    };

                    LUT()
                    {
                        table.add( "usage", USAGE );
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

                const ast::IOD::FunctionalGroupReference& fg = static_cast<const ast::IOD::FunctionalGroupReference&>( m_String );

                switch( lut[name] )
                {
                case LUT::SHARED_TYPE:
                    if( fg.modifiers & ast::AttributeSet::SHARED )
                    {
                        if( fg.modifiers & ast::AttributeSet::PER_FRAME )
                            return new CharPointer( "EFGS_BOTH" );
                        else
                            return new CharPointer( "EFGS_ONLYSHARED" );
                    }
                    else if( fg.modifiers & ast::AttributeSet::PER_FRAME )
                    {
                        return new CharPointer( "EFGS_ONLYPERFRAME" );
                    }
                    else
                    {
                        return new CharPointer( "EFGS_UNKNOWN" );
                    }

                case LUT::USAGE:
                    switch( fg.usage )
                    {
                    case ast::IOD::MANDATORY:
                        return new CharPointer( "M" );
                    case ast::IOD::CONDITIONAL:
                        return new CharPointer( "C" );
                    case ast::IOD::USER_OPTION:
                        return new CharPointer( "U" );
                    }

                default:
                    break;
                }
                return Component();
            }
        };

        IOD::IOD( const ast::IOD& iod )
        : m_IOD( iod )
        {

        }

        Component IOD::findComponent( const std::string& name )
        {
            static struct LUT
            {
                enum Field
                {
                    UNKNONW,
                    TEMPLATE_PARAMETERS,
                    BASE,
                    KEYWORD,
                    NAME,
                    MODULES,
                    FUNCTIONAL_GROUPS
                };

                LUT()
                {
                    table.add( "templateParameters", TEMPLATE_PARAMETERS );
                    table.add( "base", BASE );
                    table.add( "keyword", KEYWORD );
                    table.add( "name", NAME );
                    table.add( "modules", MODULES );
                    table.add( "functional_groups", FUNCTIONAL_GROUPS );
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
            case LUT::TEMPLATE_PARAMETERS:
                return Component( make_container<StringReference>( m_IOD.templateParameters ) );
            case LUT::BASE:
                return new StringReference( m_IOD.base );
            case LUT::KEYWORD:
                return new StringReference( m_IOD.keyword );
            case LUT::NAME:
                return new StringReference( m_IOD.name );
            case LUT::MODULES:
                return Component( make_container<ModuleReference>( m_IOD.modules ) );
            case LUT::FUNCTIONAL_GROUPS:
                return Component( make_container<FunctionalGroupReference>( m_IOD.functionalGroups ) );
            default:
                return Component();
            }
        }
    }
}
