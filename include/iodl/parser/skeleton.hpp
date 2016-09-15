#pragma once
#include <boost/spirit/include/classic_file_iterator.hpp>
#include "iodl/status.hpp"
#include "iodl/generator/component.hpp"

namespace iodl
{
    namespace parser
    {
        class Skeleton
        {
        public:
            Skeleton( const std::string& fileName );

            operator bool() const;
            bool operator!() const;

            Status operator()( iodl::generator::Component& ast );

        private:
            boost::spirit::classic::file_iterator<> m_File;
            std::string m_FileName;
        };
    }
}
