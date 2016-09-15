#pragma once
#include <vector>
#include "iodl/parser/grammar/identifier.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct TemplateDeclaration : boost::spirit::qi::grammar
    <
        Iterator,
        std::vector<std::string>(),
        Skipper
    >
    {
        TemplateDeclaration( Identifier<Iterator>& identifier ) : TemplateDeclaration::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::lexeme;

            start = lit( "template" ) >> lit( '<' ) >> parameters >> lit( '>' );

            parameters = parameter % lit( ',' );

            parameter = lexeme[lit( "typename" ) >> !identifier.idChar] >> identifier;

            start.name( "template declaration" );
            parameters.name( "template paramenter declaration sequence" );
            parameter.name( "template parameter declaration" );
        }

        boost::spirit::qi::rule<Iterator,std::vector<std::string>(),Skipper> start, parameters;
        boost::spirit::qi::rule<Iterator,std::string(),Skipper> parameter;
    };
}}}

