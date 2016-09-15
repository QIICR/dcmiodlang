#pragma once
#include <string>
#include <boost/spirit/include/qi.hpp>

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator>
    struct Identifier : boost::spirit::qi::grammar<Iterator,std::string()>
    {
        Identifier() : Identifier::base_type( start )
        {
            using boost::spirit::qi::alnum;
            using boost::spirit::qi::alpha;
            using boost::spirit::qi::char_;

            start = startChar >> *idChar;

            startChar = alpha | char_( '_' );

            idChar = alnum | char_( '_' );

            start.name( "identifier" );
        }

        boost::spirit::qi::rule<Iterator,std::string()> start;
        boost::spirit::qi::rule<Iterator,char()> startChar, idChar;
    };
}}}

