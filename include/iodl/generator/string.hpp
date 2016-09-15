#pragma once
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace generator
    {
        class CharPointer : public Component::Base
        {
        public:
            CharPointer( const char* str );
            CharPointer( const char* str, std::size_t size );
            virtual Status generate( Context& context );

        protected:
            const char* const m_String;
            const std::size_t m_Size;
        };

        class StringReference : public Component::Base
        {
        public:
            StringReference( const std::string& str );
            virtual Status generate( Context& context );

        protected:
            const std::string& m_String;
        };
    }
}
