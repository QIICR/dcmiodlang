#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include "iodl/generator/attribute.hpp"
#include "iodl/generator/string.hpp"
#include "iodl/generator/value_multiplicity.hpp"
#include "iodl/generator/value_representation.hpp"
#include "dcmtk/dcmdata/dcvr.h"

namespace iodl
{
    namespace generator
    {
        Attribute::Attribute( const ast::Attribute& attribute )
        : Attribute::Container( attribute )
        {

        }

        Component Attribute::findComponent( const std::string& name )
        {
            typedef std::vector<ast::Attribute::Argument>::const_iterator ArgumentIterator;
            static struct LUT
            {
                enum Field
                {
                    UNKNONW,
                    NAME,
                    KEYWORD,
                    VR,
                    VM,
                    TYPE,
                    DATA_TYPE
                };

                LUT()
                {
                    table.add( "name", NAME );
                    table.add( "keyword", KEYWORD );
                    table.add( "vr", VR );
                    table.add( "vm", VM );
                    table.add( "type", TYPE );
                    table.add( "dataType", DATA_TYPE );
                }

                Field operator[]( const std::string& name )
                {
                    unsigned f;
                    std::string::const_iterator it = name.begin();
                    return boost::spirit::qi::parse( it, name.end(), table, f ) && it == name.end() ? static_cast<Field>( f ) : UNKNONW;
                }

                boost::spirit::qi::symbols<char,unsigned> table;
            } lut;

            if( !m_Container.dictionaryEntry )
                return Component();

            switch( lut[name] )
            {
            case LUT::NAME:
                return new CharPointer( m_Container.dictionaryEntry->getTagName() );

            case LUT::KEYWORD:
                return new StringReference( m_Container.keyword );

            case LUT::VR:
                return new ValueRepresentation( m_Container.dictionaryEntry->getEVR() );

            case LUT::VM:
                {
                    for
                    (
                        ArgumentIterator it = m_Container.arguments.begin();
                        it != m_Container.arguments.end();
                        ++it
                    )
                    if( const ast::ValueMultiplicity* vm = boost::get<ast::ValueMultiplicity>( &*it ) )
                    {
                        return new ValueMultiplicity( *vm );
                    }
                    boost::shared_ptr<ValueMultiplicity> pVM = boost::make_shared<ValueMultiplicity>();
                    pVM->min = m_Container.dictionaryEntry->getVMMin();
                    if( m_Container.dictionaryEntry->isVariableRangeVM() )
                    {
                        pVM->max = 0;
                        pVM->increment = 1; // NOT available in DCMTK ?!
                    }
                    else
                    {
                        pVM->max = m_Container.dictionaryEntry->getVMMax();
                        pVM->increment = 1;
                    }
                    return Component( pVM );
                }

            case LUT::TYPE:
                switch( m_Container.type )
                {
                case ast::Attribute::type1:
                    return new CharPointer( "1" );
                case ast::Attribute::type1C:
                    return new CharPointer( "1C" );
                case ast::Attribute::type2:
                    return new CharPointer( "2" );
                case ast::Attribute::type2C:
                    return new CharPointer( "2C" );
                case ast::Attribute::type3:
                    return new CharPointer( "3" );
                }

            case LUT::DATA_TYPE:
                switch( m_Container.dictionaryEntry->getEVR() )
                {
                case EVR_AE:
                case EVR_CS:
                case EVR_PN:
                case EVR_DA:
                case EVR_DS:
                case EVR_DT:
                case EVR_IS:
                case EVR_SH:
                case EVR_ST:
                case EVR_TM:
                case EVR_UC:
                case EVR_UT:
                case EVR_LO:
                    return new CharPointer( "OFString" );

                case EVR_SQ:
                    for
                    (
                        ArgumentIterator it = m_Container.arguments.begin();
                        it != m_Container.arguments.end();
                        ++it
                    )
                    if( const ast::Attribute::MacroReference* macro = boost::get<ast::Attribute::MacroReference>( &*it ) )
                    {
                        return new StringReference( *macro );
                    }
                    return new CharPointer( "DcmSequenceOfItems" );

                case EVR_US:
                    return new CharPointer( "Uint16" );

                case EVR_AS:
                case EVR_AT:
                case EVR_FL:
                case EVR_FD:
                case EVR_LT:
                case EVR_OB:
                case EVR_OD:
                case EVR_OF:
                case EVR_OL:
                case EVR_OW:
                case EVR_SL:
                case EVR_SS:
                case EVR_UI:
                case EVR_UL:
                case EVR_UR:
                case EVR_ox:
                case EVR_xs:
                case EVR_lt:
                case EVR_na:
                case EVR_up:
                case EVR_item:
                case EVR_metainfo:
                case EVR_dataset:
                case EVR_fileFormat:
                case EVR_dicomDir:
                case EVR_dirRecord:
                case EVR_pixelSQ:
                case EVR_pixelItem:
                case EVR_UNKNOWN:
                case EVR_UN:
                case EVR_PixelData:
                case EVR_OverlayData:
                case EVR_UNKNOWN2B:

                default:
                    return Component();
                }

            default:
                return Component();
            }
        }
    }
}
