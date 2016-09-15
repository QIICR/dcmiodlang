#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/ast/attribute_set.hpp"
#include "iodl/parser/grammar/attribute.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct AttributeSet : boost::spirit::qi::grammar<Iterator,ast::AttributeSet(Status&),Skipper>
    {
        AttributeSet() : AttributeSet::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::eps;
            using boost::spirit::qi::space;
            using boost::spirit::qi::omit;
            using boost::spirit::qi::lexeme;
            using boost::phoenix::bind;
            using boost::phoenix::static_cast_;
            using namespace boost::spirit::qi::labels;

            start =  modifiers[bind(&ast::AttributeSet::modifiers,_val)=_1]
                  >> flavor[bind(&ast::AttributeSet::flavor,_val)=_1]
                  >> modifiers[bind(&ast::AttributeSet::modifiers,_val)|=_1]
                  >> attribute.keyword[bind(&ast::AttributeSet::keyword,_val)=_1]
                  >> attribute.text[bind(&ast::AttributeSet::name,_val)=_1]
                  >> attribute.attributes(_r1)[static_cast_<std::vector<ast::Attribute>&>(_val)=_1]
                  ;

            modifiers = eps[_val=0] >> *modifier[_val|=_1];

            modifier.add( "shared", ast::AttributeSet::SHARED );
            modifier.add( "per_frame", ast::AttributeSet::PER_FRAME );

            flavor.add( "macro", ast::AttributeSet::MACRO );
            flavor.add( "module", ast::AttributeSet::MODULE );
            flavor.add( "functional_group", ast::AttributeSet::FUNCTIONAL_GROUP );
        }

        boost::spirit::qi::rule<Iterator,ast::AttributeSet(Status&),Skipper> start;
        boost::spirit::qi::rule<Iterator,unsigned(),Skipper> modifiers;
        boost::spirit::qi::symbols<char,ast::AttributeSet::Modifier> modifier;
        boost::spirit::qi::symbols<char,ast::AttributeSet::Flavor> flavor;
        Attribute<Iterator,Skipper> attribute;
    };
}}}
