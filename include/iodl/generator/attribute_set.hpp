#pragma once
#include "iodl/ast/attribute_set.hpp"
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        class AttributeSet : public Component::Base
        {
        public:
            AttributeSet( const ast::AttributeSet& );

            virtual Component findComponent( const std::string& name );
            virtual ComponentIterator begin();
            virtual ComponentIterator end();

        private:
            struct Iterator;
            struct Flavor;
            struct Modifiers;

            const ast::AttributeSet& m_AttributeSet;
        };
    }
}
