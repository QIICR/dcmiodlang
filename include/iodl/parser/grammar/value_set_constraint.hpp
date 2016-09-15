#pragma once
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "iodl/ast/value_set_constraint.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct ValueSetConstraint : boost::spirit::qi::grammar
    <
        Iterator,
        ast::ValueSetConstraint(),
        Skipper
    >
    {
        ValueSetConstraint() : ValueSetConstraint::base_type( start )
        {
            using boost::spirit::qi::lit;
            using boost::phoenix::bind;
            using namespace boost::spirit::qi::labels;

            start = boost::spirit::qi::eps;//TODO
        }

        boost::spirit::qi::rule<Iterator,ast::ValueSetConstraint(),Skipper> start;

    };
}}}
