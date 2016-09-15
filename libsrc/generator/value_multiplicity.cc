#include <boost/make_shared.hpp>
#include <sstream>
#include "iodl/generator/value_multiplicity.hpp"
#include "iodl/generator/context.hpp"

namespace iodl
{
    namespace generator
    {
        ValueMultiplicity::ValueMultiplicity( const ast::ValueMultiplicity& rhs )
        : ast::ValueMultiplicity( rhs )
        {

        }

        std::string ValueMultiplicity::toString() const
        {
            std::ostringstream vm;
            if( min || min != max )
            {
                vm << min;
                if( min != max )
                {
                    vm << '-';
                    if( !max )
                    {
                        if( increment != 1 )
                            vm << increment;
                        vm << 'n';
                    }
                    else vm << max;
                }
            }
            else
            {
                vm << "0-";
                if( increment != 1 )
                    vm << increment;
                vm << 'n';
            }
            return vm.str();
        }

        Status ValueMultiplicity::generate( Context& context )
        {
            const std::string vm = toString();
            if( !context.write( vm.c_str(), vm.size() ) )
                return Status( "failed to write VM \"" + vm + "\" to output" );
            return Status();
        }

        std::pair<boost::shared_ptr<Component::Base>,Status>
        ValueMultiplicity::compare( const Comparable& rhs )
        {
            if( const ValueMultiplicity* pvm = dynamic_cast<const ValueMultiplicity*>( &rhs ) )
                return std::make_pair( min == pvm->min && max == pvm->max && increment == pvm->increment ? True : False, Status() );
            std::ostringstream out;
            out << "cannot compare the VM \"" << toString() << "\" to an incompatible expression";
            return std::make_pair( boost::shared_ptr<Component::Base>(), Status( out.str() ) );
        }

        bool ValueMultiplicity::result() const
        {
            return increment && ( min || max || increment != 1 );
        }
    }
}
