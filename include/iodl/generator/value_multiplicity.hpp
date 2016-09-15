#pragma once
#include "iodl/ast/value_multiplicity.hpp"
#include "iodl/generator/expression.hpp"

namespace iodl
{
    namespace generator
    {
        class ValueMultiplicity
        : public Boolean
        , public ast::ValueMultiplicity
        {
        public:
            ValueMultiplicity( const ast::ValueMultiplicity& = ast::ValueMultiplicity() );

            std::string toString() const;

            virtual Status generate( Context& context );
            virtual std::pair<boost::shared_ptr<Base>,Status> compare( const Comparable& rhs );
            virtual bool result() const;
        };
    }
}
