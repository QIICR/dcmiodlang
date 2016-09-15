#include <sstream>
#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include "iodl/generator/value_representation.hpp"
#include "iodl/generator/string.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace generator
    {
        struct ValueRepresentation::vr_lut : Component::Base
        {
            virtual Component findComponent( const std::string& name )
            {
                DcmVR vr( name.c_str() );
                if( vr.isStandard() )
                    return new ValueRepresentation( vr.getEVR() );
                return Component();
            }
        };

        Component ValueRepresentation::VR( new vr_lut );

        ValueRepresentation::ValueRepresentation( DcmEVR evr )
        : evr( evr )
        {

        }

        Component ValueRepresentation::findComponent( const std::string& name )
        {
            static struct LUT
            {
                enum Field
                {
                    UNKNONW,
                    DCMTK_CLASS,
                    IS_STRING_BASED,
                    CAN_CHECK_VM
                };

                LUT()
                {
                    table.add( "dcmtkClass", DCMTK_CLASS );
                    table.add( "isStringBased", IS_STRING_BASED );
                    table.add( "canCheckVM", CAN_CHECK_VM );
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
            case LUT::DCMTK_CLASS: // TODO
                switch( evr )
                {
                case EVR_AE: return new CharPointer( "DcmApplicationEntity" );
                case EVR_CS: return new CharPointer( "DcmCodeString" );
                case EVR_PN: return new CharPointer( "DcmPersonName" );
                case EVR_DA: return new CharPointer( "DcmDate" );
                case EVR_DS: return new CharPointer( "DcmDecimalString" );
                case EVR_DT: return new CharPointer( "DcmDateTime" );
                case EVR_IS: return new CharPointer( "DcmIntegerString" );
                case EVR_SH: return new CharPointer( "DcmShortString" );
                case EVR_ST: return new CharPointer( "DcmShortText" );
                case EVR_TM: return new CharPointer( "DcmTime" );
                case EVR_UC: return new CharPointer( "DcmUnlimitedCharacters" );
                case EVR_UT: return new CharPointer( "DcmUnlimitedText" );
                case EVR_AS: return new CharPointer( "DcmAgeString" );
                case EVR_AT: return new CharPointer( "DcmAttributeTag" );
                case EVR_FL: return new CharPointer( "DcmFloatingPointSingle" );
                case EVR_FD: return new CharPointer( "DcmFloatingPointDouble" );
                case EVR_LO: return new CharPointer( "DcmLongString" );
                case EVR_LT: return new CharPointer( "DcmLongText" );


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
                case EVR_US: return new CharPointer( "DcmUnsignedLong" );
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

                case EVR_SQ:
                default:
                    return Component();
                }

            case LUT::IS_STRING_BASED: // TODO
                switch( evr )
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
                    return Component( Boolean::True );

                default:
                    return Component( Boolean::False );
                }

            case LUT::CAN_CHECK_VM: // TODO
                switch( evr )
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
                    return Component( Boolean::True );

                default:
                    return Component( Boolean::False );
                }

            default:
                return Component();
            }
        }

        Status ValueRepresentation::generate( Context& context )
        {
            DcmVR vr( evr );
            if( vr.isStandard() )
            {
                if( !context.write( vr.getVRName(), strlen( vr.getVRName() ) ) )
                    return Status( "failed to write VR \"" + std::string( vr.getVRName() ) + "\" to output" );
            }
            else return Base::generate( context );
            return Status();
        }

        std::pair<boost::shared_ptr<Component::Base>,Status>
        ValueRepresentation::compare( const Comparable& rhs )
        {
            if( const ValueRepresentation* pvr = dynamic_cast<const ValueRepresentation*>( &rhs ) )
                return std::make_pair( evr == pvr->evr ? True : False, Status() );
            std::ostringstream out;
            out << "cannot compare the VR \"" << DcmVR( evr ).getVRName() << "\" to an incompatible expression";
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( out.str() ) );
        }

        bool ValueRepresentation::result() const
        {
            return DcmVR( evr ).isStandard();
        }
    }
}
