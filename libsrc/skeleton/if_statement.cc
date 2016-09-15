#include <boost/make_shared.hpp>
#include <sstream>
#include "iodl/skeleton/if_statement.hpp"
#include "iodl/generator/expression.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace skeleton
    {
        IfStatement::IfStatement( const Status::FilePosition& position )
        : Statement( position )
        {

        }

        Status IfStatement::generate( generator::Context& context )
        {
            std::pair<generator::Component,Status> r = condition( context );
            if( !r.second )
                return r.second;
            if( generator::Boolean* p = r.first.as<generator::Boolean>() )
            {
                if( p->result() )
                    return then_statement.generate( context );
                else if( else_statement )
                    return else_statement.generate( context );
                return Status();
            }
            std::ostringstream out;
            out << "the condition \"";
            generator::StackedOutputStreamContext dump( context, out );
            r.first.generate( dump );
            out << "\" is no boolean expression";
            return Status( out.str(), position );
        }
    }
}
