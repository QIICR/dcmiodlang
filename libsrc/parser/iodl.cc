#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/filesystem.hpp>
#include "iodl/parser/grammar/iodl.hpp"
#include "iodl/parser/iodl.hpp"
#include "iodl/parser/next_token.hpp"

namespace iodl
{
    namespace parser
    {
        IODL::IODL( const std::string& filename )
        : m_File( filename )
        , m_FileName( boost::filesystem::absolute( filename ).native() )
        {

        }

        IODL::operator bool() const
        {
            return m_File;
        }

        bool IODL::operator!() const
        {
            return !m_File;
        }

        Status IODL::operator()( ast::IODL& ast )
        {
            typedef boost::spirit::classic::position_iterator2
            <
                boost::spirit::classic::file_iterator<>
            > iterator_type;
            static grammar::IODL
            <
                iterator_type,
                boost::spirit::qi::space_type
            > grammar;
            iterator_type it
            (
                m_File,
                m_File.make_end(),
                m_FileName
            );
            Status status;
            if
            (
                !boost::spirit::qi::phrase_parse
                (
                    it,
                    iterator_type(),
                    grammar(boost::phoenix::ref(status)),
                    boost::spirit::qi::space,
                    ast
                ) || it != iterator_type()
            )
            {
                if( !status )
                    return status;
                return Status
                (
                    "Unknown keyword or type of entity \"" + nextToken( it, iterator_type() ) + '"',
                    boost::make_shared<Status::FilePosition>( it.get_position() )
                );
            }
            return status;
        }
    }
}
