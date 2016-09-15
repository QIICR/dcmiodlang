#include <boost/make_shared.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/filesystem.hpp>
#include "iodl/parser/grammar/skeleton.hpp"
#include "iodl/parser/skeleton.hpp"

namespace iodl
{
    namespace parser
    {
        Skeleton::Skeleton( const std::string& filename )
        : m_File( filename )
        , m_FileName( boost::filesystem::absolute( filename ).native() )
        {

        }

        Skeleton::operator bool() const
        {
            return m_File;
        }

        bool Skeleton::operator!() const
        {
            return !m_File;
        }

        Status Skeleton::operator()( generator::Component& ast )
        {
            typedef boost::spirit::classic::position_iterator2
            <
                boost::spirit::classic::file_iterator<>
            > iterator_type;
            static grammar::Skeleton<iterator_type> grammar;
            iterator_type it
            (
                m_File,
                m_File.make_end(),
                m_FileName
            );
            if
            (
                !boost::spirit::qi::phrase_parse
                (
                    it,
                    iterator_type(),
                    grammar,
                    boost::spirit::qi::space,
                    ast
                ) || it != iterator_type()
            )
            {
                iterator_type t = it;
                std::string nextToken;
                boost::spirit::qi::phrase_parse
                (
                    t,
                    iterator_type(),
                    boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - boost::spirit::qi::space)],
                    boost::spirit::qi::space,
                    nextToken
                );
                return Status
                (
                    "Unexpected token \"" + nextToken + '"',
                    boost::make_shared<Status::FilePosition>( it.get_position() )
                );
            }
            return Status();
        }
    }
}
