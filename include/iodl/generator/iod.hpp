#pragma once
#include "iodl/ast/iod.hpp"
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        class IOD : public Component::Base
        {
        public:
            IOD( const ast::IOD& iod );

            virtual Component findComponent( const std::string& name );

        private:
            struct ModuleReference;
            struct FunctionalGroupReference;

            const ast::IOD& m_IOD;
        };
    }
}
