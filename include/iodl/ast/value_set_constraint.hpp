#pragma once
#include <boost/variant.hpp>

namespace iodl
{
    namespace ast
    {
        typedef boost::variant
        <
            long long,
            double,
            std::string
        > Value;

        struct ValueSetConstraintReference : std::string
        {
            inline ValueSetConstraintReference()
            : std::string()
            {

            }

            inline ValueSetConstraintReference( const std::string& keyword )
            : std::string( keyword )
            {

            }
        };

        struct ValueSetConstraint : std::vector
        <
            boost::variant<Value,ValueSetConstraintReference>
        >
        {
            enum Flavor
            {
                ENUMERATED_VALUE,
                DEFINED_TERM
            };

            Flavor flavor;
            std::string keyword;
            std::string name;
        };
    }
}
