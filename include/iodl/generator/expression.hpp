#pragma once
#include "iodl/generator/component.hpp"
#include "iodl/generator/comparable.hpp"

namespace iodl
{
    namespace generator
    {
        class Boolean : public virtual Component::Base, public Comparable
        {
            struct true_type;
            struct false_type;

        public:
            static boost::shared_ptr<Boolean> True;
            static boost::shared_ptr<Boolean> False;

            virtual std::pair<boost::shared_ptr<Base>,Status>
            compare( const Comparable& rhs );
            virtual bool result() const = 0;
        };

        struct Not : Component::Base
        {
            Not( const Component& expression );
            virtual Status generate( Context& context );
            virtual std::pair<boost::shared_ptr<Base>,Status> evaluate( Context& context );
            Component expression;
        };

        struct Equal : Component::Base
        {
            Equal( const Component& lhs,
                   const Component& rhs );
            virtual Status generate( Context& context );
            virtual std::pair<boost::shared_ptr<Base>,Status> evaluate( Context& context );
            Component lhs;
            Component rhs;
        };

        struct NotEqual : Component::Base
        {
            NotEqual( const Component& lhs,
                      const Component& rhs );
            virtual Status generate( Context& context );
            virtual std::pair<boost::shared_ptr<Base>,Status> evaluate( Context& context );
            Component lhs;
            Component rhs;
        };
    }
}
