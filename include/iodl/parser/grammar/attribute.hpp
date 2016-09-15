#pragma once
#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/status.hpp"
#include "iodl/ast/attribute.hpp"
#include "iodl/parser/grammar/identifier.hpp"
#include "iodl/parser/grammar/value_multiplicity.hpp"
#include "iodl/parser/grammar/value_set_constraint.hpp"
#include "iodl/parser/grammar/string_literal.hpp"
#include "iodl/parser/next_token.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct Attribute : boost::spirit::qi::grammar<Iterator,ast::Attribute(Status&),Skipper>
    {
        static void handle_error( Status& status, Iterator it, Iterator end, const boost::spirit::info& what )
        {
            if( status )
            {
                std::ostringstream err;
                err << "expected " << what << ", got \"" << nextToken( it, end ) << '"';
                status = Status( err.str(), boost::make_shared<Status::FilePosition>( it.get_position() ) );
            }
        }

        static bool resolve_attribute( Status& status, ast::Attribute& attr, const boost::iterator_range<Iterator>& range )
        {
            boost::shared_ptr<Status::FilePosition> pos( new Status::FilePosition( range.begin().get_position() ) );
            ast::Attribute::LookupStatus s = attr.lookupInDictionary();
            if( s != ast::Attribute::OK )
            {
                switch( s )
                {
                case ast::Attribute::NOT_FOUND:
                    status = Status
                    (
                        std::string( "could not find the attribute \"" ) + attr.tag.toString().c_str() +
                        ' ' + attr.keyword + "\" in the data dictionary",
                        pos
                    );
                    break;
                case ast::Attribute::TAG_MISMATCH:
                    status = Status
                    (
                        std::string( "the tag " ) + attr.tag.toString().c_str() +
                        " does not match the attribute keyword \"" + attr.keyword + '"',
                        pos
                    );
                    break;
                default:
                    break;
                }
                return false;
            }
            return true;
        }

        static bool check_sq_vr( Status& status, ast::Attribute& attr, const boost::iterator_range<Iterator>& range )
        {
            if( attr.dictionaryEntry->getEVR() != EVR_SQ )
            {
                status = Status
                (
                    "only attributes with VR=SQ may contain nested attributes",
                    boost::make_shared<Status::FilePosition>( range.begin().get_position() )
                );
                return false;
            }
            return true;
        }

        Attribute() : Attribute::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::attr;
            using boost::spirit::qi::eps;
            using boost::spirit::qi::raw;
            using boost::phoenix::bind;
            using boost::phoenix::static_cast_;
            using namespace boost::spirit::qi::labels;

            start =  tag[bind(&ast::Attribute::tag,_val) = _1]
                  >  keyword[bind(&ast::Attribute::keyword,_val) = _1]
                  >  raw[eps][boost::phoenix::arg_names::_3 = bind(&resolve_attribute,_r1,_val,_1)]
                  >> -arguments[bind(&ast::Attribute::arguments,_val) = _1]
                  >  type[bind(&ast::Attribute::type,_val) = _1]
                  >> -
                  (
                     attributes(_r1)[static_cast_<std::vector<ast::Attribute>&>(_val)=_1] >
                     raw[eps][boost::phoenix::arg_names::_3 = bind(&check_sq_vr,_r1,_val,_1)]
                  )
                  >> -text[bind(&ast::Attribute::text,_val) = _1]
                  ;

            tag =  lit( '(' )
                >> uint16[bind(&DcmTagKey::setGroup,_val,_1)]
                >> lit( ',' )
                >> uint16[bind(&DcmTagKey::setElement,_val,_1)]
                >> lit( ')' )
                ;

            type = lit( "type" ) >>
                 (
                     ( lit( '1' ) >> ( ( lit( 'C' ) >> attr( ast::Attribute::type1C ) ) | attr( ast::Attribute::type1 ) ) ) |
                     ( lit( '2' ) >> ( ( lit( 'C' ) >> attr( ast::Attribute::type2C ) ) | attr( ast::Attribute::type2 ) ) ) |
                     ( lit( '3' ) >> attr( ast::Attribute::type3 ) )
                 )
                 ;

            arguments = lit( '(' ) >> ( argument % ',' ) >> lit( ')' );

            argument = ( lit( "items" ) >> lit( '=' ) >> macro )
                     | ( lit( "VM" ) >> lit( '=' ) >> vm )
                     | vsc
                     ;

            macro = keyword;

            attributes = lit( '{' ) >> *( start(_r1) > lit( ';' ) ) > lit( '}' );

            start.name( "an attribute" );
            tag.name( "a DICOM attribute tag" );

            boost::spirit::qi::on_error<boost::spirit::qi::fail>
            (
                start,
                bind( &handle_error, _r1, _3, _2, _4 )
            );

            boost::spirit::qi::on_error<boost::spirit::qi::fail>
            (
                attributes,
                bind( &handle_error, _r1, _3, _2, _4 )
            );
        }

        boost::spirit::qi::rule<Iterator,ast::Attribute(Status&),Skipper> start;
        boost::spirit::qi::rule<Iterator,DcmTagKey(),Skipper> tag;
        boost::spirit::qi::rule<Iterator,ast::Attribute::Type(),Skipper> type;
        boost::spirit::qi::rule<Iterator,std::vector<ast::Attribute::Argument>(),Skipper> arguments;
        boost::spirit::qi::rule<Iterator,ast::Attribute::Argument(),Skipper> argument;
        boost::spirit::qi::rule<Iterator,ast::Attribute::MacroReference(),Skipper> macro;
        boost::spirit::qi::rule<Iterator,std::vector<ast::Attribute>(Status&),Skipper> attributes;
        Identifier<Iterator> keyword;
        ValueMultiplicity<Iterator,Skipper> vm;
        ValueSetConstraint<Iterator,Skipper> vsc;
        StringLiteral<Iterator,Skipper> text;
        boost::spirit::qi::uint_parser<Uint16,16> uint16;
    };
}}}
