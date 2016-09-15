#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/ast/iod.hpp"
#include "iodl/parser/grammar/identifier.hpp"
#include "iodl/parser/grammar/string_literal.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct IOD : boost::spirit::qi::grammar<Iterator,ast::IOD(),Skipper>
    {
        IOD() : IOD::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::eps;
            using boost::phoenix::bind;
            using boost::phoenix::static_cast_;
            using boost::phoenix::push_back;
            using namespace boost::spirit::qi::labels;

            start =  lit( "iod" )
                  >> keyword[bind(&ast::IOD::keyword,_val)=_1]
                  >> stringLiteral[bind(&ast::IOD::name,_val)=_1]
                  >> lit( ':' ) >> keyword[bind(&ast::IOD::base,_val)=_1]
                  >> lit( '{' )
                  >>    *section(_val)
                  >> lit( '}' )
                  ;

            section = ( lit( "modules" ) >> lit( ':' ) >> modules(bind(&ast::IOD::modules,_r1)) )
                    | ( lit( "functional_groups" ) >> lit( ':' ) >> functional_groups(bind(&ast::IOD::functionalGroups,_r1)) )
                    ;

            modules = *module[push_back(_r1,_1)];

            functional_groups = *functional_group[push_back(_r1,_1)];

            module =  keyword[static_cast_<std::string&>(_val)=_1]
                   >> usage[bind(&ast::IOD::ModuleReference::usage,_val)=_1]
                   >> lit( ';' )
                   ;

            functional_group =  modifiers[bind(&ast::IOD::FunctionalGroupReference::modifiers,_val)=_1]
                             >> keyword[static_cast_<std::string&>(_val)=_1]
                             >> modifiers[bind(&ast::IOD::FunctionalGroupReference::modifiers,_val)|=_1]
                             >> usage[bind(&ast::IOD::FunctionalGroupReference::usage,_val)=_1]
                             >> lit( ';' )
                             ;

            modifiers = eps[_val=0] >> *modifier[_val|=_1];

            modifier.add( "shared", ast::IOD::SHARED );
            modifier.add( "per_frame", ast::IOD::PER_FRAME );

            usage.add( "mandatory", ast::IOD::MANDATORY )( "M", ast::IOD::MANDATORY );
            usage.add( "conditional", ast::IOD::CONDITIONAL )( "C", ast::IOD::CONDITIONAL );
            usage.add( "user_option", ast::IOD::USER_OPTION )( "optional", ast::IOD::USER_OPTION )( "U", ast::IOD::USER_OPTION );
        }

        boost::spirit::qi::rule<Iterator,ast::IOD(),Skipper> start;
        boost::spirit::qi::rule<Iterator,void(ast::IOD&),Skipper> section;
        boost::spirit::qi::rule<Iterator,void(std::vector<ast::IOD::ModuleReference>&),Skipper> modules;
        boost::spirit::qi::rule<Iterator,void(std::vector<ast::IOD::FunctionalGroupReference>&),Skipper> functional_groups;
        boost::spirit::qi::rule<Iterator,ast::IOD::ModuleReference(),Skipper> module;
        boost::spirit::qi::rule<Iterator,ast::IOD::FunctionalGroupReference(),Skipper> functional_group;
        boost::spirit::qi::rule<Iterator,unsigned(),Skipper> modifiers;
        boost::spirit::qi::symbols<char,ast::IOD::Modifier> modifier;
        boost::spirit::qi::symbols<char,ast::IOD::Usage> usage;
        StringLiteral<Iterator,Skipper> stringLiteral;
        Identifier<Iterator> keyword;
    };
}}}
