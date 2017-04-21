#pragma once
#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/parser/grammar/identifier.hpp"
#include "iodl/parser/grammar/value_multiplicity.hpp"
#include "iodl/skeleton/block.hpp"
#include "iodl/skeleton/text.hpp"
#include "iodl/skeleton/reference.hpp"
#include "iodl/skeleton/if_statement.hpp"
#include "iodl/skeleton/for_loop.hpp"
#include "iodl/skeleton/whitespace.hpp"
#include "iodl/skeleton/toupper.hpp"
#include "iodl/skeleton/value_multiplicity.hpp"
#include "iodl/generator/expression.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator>
    struct Skeleton : boost::spirit::qi::grammar<Iterator,generator::Component()>
    {
        template<typename T>
        static boost::shared_ptr<T> make_statement( const boost::iterator_range<Iterator>& r )
        {
            return boost::make_shared<T>( r.begin().get_position() );
        }

        template<typename T>
        static boost::shared_ptr<T> make_statement_string( const boost::iterator_range<Iterator>& r, const std::string& str )
        {
            return boost::make_shared<T>( r.begin().get_position(), str );
        }

        Skeleton() : Skeleton::base_type( start )
        {
            using boost::spirit::qi::space;
            using boost::spirit::qi::eps;
            using boost::spirit::qi::raw;
            using boost::spirit::qi::lit;
            using boost::spirit::qi::skip;
            using boost::spirit::qi::char_;
            using boost::spirit::qi::alpha;
            using boost::spirit::qi::alnum;
            using boost::spirit::qi::string;
            using boost::spirit::qi::as_string;
            using boost::phoenix::bind;
            using boost::phoenix::begin;
            using boost::phoenix::static_cast_;
            using namespace boost::spirit::qi::labels;

            start = make_block(false);

            make_block =  raw[eps][_val = bind( &make_statement<skeleton::Block>, _1 )]
                       >> components(_r1)[bind( &skeleton::Block::components, _val ) = _1];

            components = *component(_r1);

            component = text(_r1,std::string()) | ( lit( '$' ) > statement(true) );

            text = raw
                 [
                     ( eps(_r1) >> inner_text[_r2 = _1] ) |
                     ( eps(!_r1) >> top_level_text[_r2 = _1] )
                 ][_val = bind( &make_statement_string<skeleton::Text>, _1, _r2 )]
                 ;

            inner_text = +( char_ - '$' - '}' | escaped_char );

            top_level_text = +( char_ - '$' | escaped_char );

            escaped_char = lit( '$' ) >> char_( "}$" );

            statement = if_statment | for_loop | whitespace | toupper | block |
                      (
                          ( eps(_r1) >> lit( '<' ) >> expression >> lit( '>' ) ) |
                          ( eps(!_r1) >> expression )
                      )
                      ;

            expression = lhs[_val = _1] >> *skip(space)
                       [
                           lit( "==" ) >> lhs[_val=bind(&boost::make_shared<generator::Equal,const generator::Component&,const generator::Component&>,_val,_1)] |
                           lit( "!=" ) >> lhs[_val=bind(&boost::make_shared<generator::NotEqual,const generator::Component&,const generator::Component&>,_val,_1)]
                       ]
                       ;

            lhs = skip(space)[lit( '!' ) >> lhs[_val=bind(&boost::make_shared<generator::Not,const generator::Component&>,_1)]]
                | vm[_val=_1]
                | reference[_val=_1]
                ;

            block = lit( '{' ) >> make_block(true) >> lit( '}' );

            if_statment = skip(space)
                     [
                         raw[lit( "if" )][_val = bind( &make_statement<skeleton::IfStatement>, _1 )] >>
                         lit( '(' ) >> expression[bind( &skeleton::IfStatement::condition, _val ) = _1] >>
                         lit( ')' ) >> statement(true)[bind( &skeleton::IfStatement::then_statement, _val ) = _1] >> -
                         (
                            lit( "$else" ) >> statement(true)[bind( &skeleton::IfStatement::else_statement, _val ) = _1]
                         )
                     ]
                     ;

            for_loop = skip(space)
                     [
                         raw[lit( "for" )][_val = bind( &make_statement<skeleton::ForLoop>, _1 )] >>
                         lit( '(' ) >> identifier[bind( &skeleton::ForLoop::name, _val ) = _1] >>
                         lit( ':' ) >> expression[bind( &skeleton::ForLoop::range, _val ) = _1] >>
                         lit( ')' ) >> statement(true)[bind( &skeleton::ForLoop::statement, _val ) = _1]
                     ]
                     ;

            reference =  raw[eps][_val = bind( &make_statement<skeleton::Reference>, _1 )]
                      >> identifier_path[bind( &skeleton::Reference::identifierPath, _val ) = _1];

            whitespace = skip(space)
                       [
                           raw[lit( "whitespace" )][_val = bind( &make_statement<skeleton::Whitespace>, _1 )] >>
                           lit( '(' ) >> statement(false)[bind( &skeleton::Whitespace::statement, _val ) = _1] >> lit( ')' )
                       ];

            toupper = skip(space)
                    [
                        raw[lit( "toupper" )][_val = bind( &make_statement<skeleton::Toupper>, _1 )] >>
                        lit( '(' ) >> statement(false)[bind( &skeleton::Toupper::statement, _val ) = _1] >> lit( ')' )
                    ];

            vm = skip(space)
               [
                   raw[lit( "VM" )][_val = bind(&make_statement<skeleton::ValueMultiplicity>, _1)] >> lit( '(' ) >>
                   value_multiplicity[static_cast_<ast::ValueMultiplicity&>(*_val) = _1] >> lit( ')' )
               ];

            identifier_path = identifier % lit( '.' );
        }

        boost::spirit::qi::rule<Iterator,generator::Component()> start, expression, lhs;
        boost::spirit::qi::rule<Iterator,generator::Component(bool)> component, statement;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Block>()> block;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Block>(bool)> make_block;
        boost::spirit::qi::rule<Iterator,std::vector<generator::Component>(bool)> components;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Text>(bool,std::string)> text;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::IfStatement>()> if_statment;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::ForLoop>()> for_loop;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Reference>()> reference;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Whitespace>()> whitespace;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::Toupper>()> toupper;
        boost::spirit::qi::rule<Iterator,boost::shared_ptr<skeleton::ValueMultiplicity>()> vm;
        boost::spirit::qi::rule<Iterator,std::vector<std::string>()> identifier_path;
        boost::spirit::qi::rule<Iterator,std::string()> inner_text, top_level_text;
        boost::spirit::qi::rule<Iterator,char()> escaped_char;
        Identifier<Iterator> identifier;
        ValueMultiplicity<Iterator,boost::spirit::qi::space_type> value_multiplicity;
    };
}}}
