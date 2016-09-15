#pragma once
#include "iodl/ast/attribute.hpp"
#include "iodl/generator/container.hpp"

namespace iodl
{
    namespace generator
    {
        class Attribute : public Container<ast::Attribute,Attribute>
        {
        public:
            Attribute( const ast::Attribute& );

            virtual Component findComponent( const std::string& name );
        };
    }
}
