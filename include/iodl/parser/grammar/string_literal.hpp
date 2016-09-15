#pragma once
#include <string>
#include <boost/spirit/include/qi.hpp>

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct StringLiteral : boost::spirit::qi::grammar<Iterator,std::string(),Skipper>
    {
        StringLiteral() : StringLiteral::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::char_;
            using boost::spirit::qi::lexeme;

            start = +quoted_string;

            quoted_string = lit( '"' ) >> lexeme
                          [
                              *( char_ - '"' | ( lit( '\\' ) >> char_( '"' ) ) )
                          ] >> lit( '"' );
        }

        boost::spirit::qi::rule<Iterator,std::string(),Skipper> start, quoted_string;
    };
}}}

