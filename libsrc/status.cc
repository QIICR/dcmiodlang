#include <boost/make_shared.hpp>
#include "iodl/status.hpp"

namespace iodl
{
    std::ostream& operator<<( std::ostream& out, const Status& status )
    {
        if( !status.m_Stack )
            return out << "success";
        status.m_Stack->print( out );
        return out;
    }

    Status::Base::~Base()
    {

    }

    Status::FilePosition::FilePosition( const boost::spirit::classic::file_position& pos )
    : boost::spirit::classic::file_position( pos )
    {

    }

    void Status::FilePosition::print( std::ostream& out )
    {
        out << file << ':' << line << ':' << column;
    }

    Status::Stack::Stack( const boost::shared_ptr<Base>& status,
                          const boost::shared_ptr<Base>& stack )
    : status( status )
    , stack( stack )
    {

    }

    void Status::Stack::print( std::ostream& out )
    {

    }

    Status::Error::Error( const std::string& message,
                          const boost::shared_ptr<Base>& stack )
    : message( message )
    , stack( stack )
    {

    }

    void Status::Error::print( std::ostream& out )
    {
        if( stack )
        {
            stack->print( out );
            out << ": ";
        }
        out << "error: " << message;
    }

    Status::Status()
    : m_Stack()
    {

    }

    Status::Status( const std::string& errorMessage,
                    const boost::shared_ptr<FilePosition>& filePosition )
    : m_Stack( new Error( errorMessage, filePosition ) )
    {

    }

    Status::operator bool() const
    {
        return !m_Stack;
    }

    bool Status::operator!() const
    {
        return !!m_Stack;
    }

    Status Status::operator+( const Status& rhs ) const
    {
        if( !rhs )
            return *this;
        if( !*this )
            return rhs;
        Status s;
        s.m_Stack = boost::make_shared<Stack>( m_Stack, rhs.m_Stack );
        return s;
    }

    Status Status::operator+( const boost::shared_ptr<FilePosition>& filePosition ) const
    {
        if( !*this )
            return Status();
        Status s;
        s.m_Stack = boost::make_shared<Stack>( m_Stack, filePosition );
        return s;
    }
}
