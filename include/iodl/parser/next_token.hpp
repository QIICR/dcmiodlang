#pragma once
#include <boost/spirit/include/qi.hpp>

namespace iodl
{
    namespace parser
    {
        template<typename Iterator>
        std::string nextToken( Iterator it, const Iterator& end )
        {
            std::string token;
            boost::spirit::qi::phrase_parse
            (
                it,
                end,
                boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - boost::spirit::qi::space)],
                boost::spirit::qi::space,
                token
            );
            return token;
        }
    }
}
