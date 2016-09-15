#pragma once
#include <ostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace iodl
{
    class Status
    {
    public:
        friend std::ostream& operator<<( std::ostream& out, const Status& status );

        struct Base
        {
            virtual void print( std::ostream& out ) = 0;
            virtual ~Base();
        };

        struct FilePosition : boost::spirit::classic::file_position, Base
        {
            FilePosition( const boost::spirit::classic::file_position& pos );
            virtual void print( std::ostream& out );
        };

        struct Stack : Base
        {
            Stack( const boost::shared_ptr<Base>& status,
                   const boost::shared_ptr<Base>& stack );
            virtual void print( std::ostream& out );
            boost::shared_ptr<Base> status;
            boost::shared_ptr<Base> stack;
        };

        struct Error : Base
        {
            Error( const std::string& message,
                   const boost::shared_ptr<Base>& stack );
            virtual void print( std::ostream& out );
            std::string             message;
            boost::shared_ptr<Base> stack;
        };

        Status();
        explicit Status( const std::string& errorMessage,
                         const boost::shared_ptr<FilePosition>& filePosition = boost::shared_ptr<FilePosition>() );

        operator bool() const;
        bool operator!() const;

        Status operator+( const Status& rhs ) const;
        Status operator+( const boost::shared_ptr<FilePosition>& filePosition ) const;

    private:
        boost::shared_ptr<Base> m_Stack;
    };
}
