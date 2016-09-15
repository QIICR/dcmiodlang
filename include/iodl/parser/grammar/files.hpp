#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include "iodl/ast/files.hpp"
#include "iodl/parser/grammar/identifier.hpp"
#include "iodl/parser/grammar/string_literal.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct Files : boost::spirit::qi::grammar
    <
        Iterator,
        ast::Files(),
        Skipper
    >
    {
        Files() : Files::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::phoenix::bind;
            using boost::phoenix::insert;
            using namespace boost::spirit::qi::labels;

            start =  lit( "files" ) >> keyword[bind(&ast::Files::keyword,_val)=_1]
                  >> lit( '{' )
                  >>   *file[insert(_val,_1)]
                  >> lit( '}' )
                  ;

            file = keyword >> lit( '=' ) >> string_literal >> lit( ';' );
        }

        boost::spirit::qi::rule<Iterator,ast::Files(),Skipper> start;
        boost::spirit::qi::rule<Iterator,std::pair<std::string,std::string>(),Skipper> file;
        Identifier<Iterator> keyword;
        StringLiteral<Iterator,Skipper> string_literal;
    };
}}}
