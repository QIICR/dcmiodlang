#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/ast/value_multiplicity.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct ValueMultiplicity : boost::spirit::qi::grammar
    <
        Iterator,
        ast::ValueMultiplicity(),
        Skipper,
        boost::spirit::qi::locals<unsigned>
    >
    {
        ValueMultiplicity() : ValueMultiplicity::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::uint_;
            using boost::spirit::qi::eps;
            using boost::spirit::qi::lexeme;
            using boost::phoenix::bind;
            using namespace boost::spirit::qi::labels;

            start = ( uint_[_a = _1] >> vm(_val,_a) )
                  | ( n(_val,1) >> eps[bind(&ast::ValueMultiplicity::min,_val)=0] )
                  ;

            vm =
               (
                   n(_r1,_r2) >> eps[bind(&ast::ValueMultiplicity::min,_r1)=0]
               )
               |
               (
                   eps[bind(&ast::ValueMultiplicity::min,_r1)=_r2] >>
                   (
                       (
                           lit( '-' ) >> eps[_r2=1] >> -uint_[_r2=_1] >> n(_r1,_r2)
                       )
                       |
                       (
                           eps[bind(&ast::ValueMultiplicity::increment,_r1)=1] >>
                           eps[bind(&ast::ValueMultiplicity::max,_r1)=_r2]
                       )
                   )
               );

            n =  lit( 'n' )[bind(&ast::ValueMultiplicity::increment,_r1)=_r2]
              >> eps[bind(&ast::ValueMultiplicity::max,_r1)=0]
              ;
        }

        boost::spirit::qi::rule
        <
            Iterator,
            ast::ValueMultiplicity(),
            Skipper,
            boost::spirit::qi::locals<unsigned>
        > start;

        boost::spirit::qi::rule
        <
            Iterator,
            void(ast::ValueMultiplicity&,unsigned),
            Skipper
        > vm, n;
    };
}}}
