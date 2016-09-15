#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include "iodl/parser/grammar/identifier.hpp"
#include "iodl/parser/grammar/string_literal.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct Property : boost::spirit::qi::grammar
    <
        Iterator,
        std::pair<std::string,std::string>(),
        Skipper
    >
    {
        Property() : Property::base_type( start )
        {
            using boost::spirit::qi::lit;

            start = lit( "property" ) >> keyword >> lit( '=' ) >> string_literal;
        }

        boost::spirit::qi::rule<Iterator,std::pair<std::string,std::string>(),Skipper> start;
        Identifier<Iterator> keyword;
        StringLiteral<Iterator,Skipper> string_literal;
    };
}}}
