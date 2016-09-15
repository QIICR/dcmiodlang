#pragma once
#include <boost/spirit/include/classic_file_iterator.hpp>
#include "iodl/status.hpp"
#include "iodl/ast/iodl.hpp"

namespace iodl
{
    namespace parser
    {
        class IODL
        {
        public:
            IODL( const std::string& fileName );

            operator bool() const;
            bool operator!() const;

            Status operator()( iodl::ast::IODL& ast );

        private:
            boost::spirit::classic::file_iterator<> m_File;
            std::string m_FileName;
        };
    }
}
