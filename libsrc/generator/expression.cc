#include <sstream>
#include "iodl/generator/expression.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace generator
    {
        struct Boolean::true_type : Boolean
        {
            virtual Status generate( Context& context )
            {
                context.write( "true", 4 );//TODO: handle error?
                return Status();
            }

            virtual bool result() const
            {
                return true;
            }
        };

        struct Boolean::false_type : Boolean
        {
            virtual Status generate( Context& context )
            {
                context.write( "false", 5 );//TODO: handle error?
                return Status();
            }

            virtual bool result() const
            {
                return false;
            }
        };

        boost::shared_ptr<Boolean> Boolean::True( new true_type );
        boost::shared_ptr<Boolean> Boolean::False( new false_type );

        std::pair<boost::shared_ptr<Component::Base>,Status>
        Boolean::compare( const Comparable& rhs )
        {
            if( const Boolean* pb = dynamic_cast<const Boolean*>( &rhs ) )
                return std::make_pair( result() == pb->result() ? True : False, Status() );
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( "type mismatch in comparison" ) );
        }

        Not::Not( const Component& expression )
        : expression( expression )
        {

        }

        Status Not::generate( Context& context )
        {
            context.put( '!' );
            expression.generate( context );
            return Status();
        }

        std::pair<boost::shared_ptr<Component::Base>,Status> Not::evaluate( Context& context )
        {
            std::pair<Component,Status> r = expression( context );
            if( !r.second )
                return std::make_pair( boost::shared_ptr<Component::Base>(), r.second );
            if( Boolean* pb = r.first.as<Boolean>() )
                return std::make_pair( pb->result() ? Boolean::False : Boolean::True, Status() );
            std::ostringstream out;
            out << "\"";
            generator::StackedOutputStreamContext dump( context, out );
            r.first.generate( dump );
            out << "\" is no boolean expression and therefore cannot be negated";
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( out.str() ) );
        }

        Equal::Equal( const Component& lhs,
                      const Component& rhs )
        : lhs( lhs )
        , rhs( rhs )
        {

        }

        Status Equal::generate( Context& context )
        {
            rhs.generate( context );
            context.write( "==", 2 );
            lhs.generate( context );
            return Status();
        }

        std::pair<boost::shared_ptr<Component::Base>,Status> Equal::evaluate( Context& context )
        {
            std::pair<Component,Status> l = lhs( context );
            if( !l.second )
                return std::make_pair( boost::shared_ptr<Component::Base>(), l.second );
            std::pair<Component,Status> r = rhs( context );
            if( !r.second )
                return std::make_pair( boost::shared_ptr<Component::Base>(), r.second );
            if( Comparable* pl = l.first.as<Comparable>() )
            if( Comparable* pr = r.first.as<Comparable>() )
                return pl->compare( *pr );
            std::ostringstream out;
            out << "the expressions \"";
            generator::StackedOutputStreamContext dump( context, out );
            l.first.generate( dump );
            out << "\" and \"";
            r.first.generate( dump );
            out << "\" are incomparable";
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( out.str() ) );
        }

        NotEqual::NotEqual( const Component& lhs,
                            const Component& rhs )
        : lhs( lhs )
        , rhs( rhs )
        {

        }

        Status NotEqual::generate( Context& context )
        {
            rhs.generate( context );
            context.write( "!=", 2 );
            lhs.generate( context );
            return Status();
        }

        std::pair<boost::shared_ptr<Component::Base>,Status> NotEqual::evaluate( Context& context )
        {
            std::pair<Component,Status> l = lhs( context );
            if( !l.second )
                return std::make_pair( boost::shared_ptr<Component::Base>(), l.second );
            std::pair<Component,Status> r = rhs( context );
            if( !r.second )
                return std::make_pair( boost::shared_ptr<Component::Base>(), r.second );
            if( Comparable* pl = l.first.as<Comparable>() )
            if( Comparable* pr = r.first.as<Comparable>() )
            {
                std::pair<boost::shared_ptr<Component::Base>,Status> x = pl->compare( *pr );
                if( !x.second )
                    return std::make_pair( boost::shared_ptr<Component::Base>(), x.second );
                if( Boolean* pb = dynamic_cast<Boolean*>( x.first.get() ) )
                    return std::make_pair( pb->result() ? Boolean::False : Boolean::True, Status() );
                return Base::evaluate( context );
            }
            std::ostringstream out;
            out << "the expressions \"";
            generator::StackedOutputStreamContext dump( context, out );
            l.first.generate( dump );
            out << "\" and \"";
            r.first.generate( dump );
            out << "\" are incomparable";
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( out.str() ) );
        }
    }
}
