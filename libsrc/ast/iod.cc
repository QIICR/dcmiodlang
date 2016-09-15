#include "iodl/ast/iod.hpp"

namespace iodl
{
    namespace ast
    {
        IOD::ModuleReference::ModuleReference( const std::string& keyword,
                                               Usage usage )
        : std::string( keyword )
        , usage( usage )
        {

        }

        IOD::FunctionalGroupReference::FunctionalGroupReference( const std::string& keyword,
                                                                 Usage usage,
                                                                 unsigned modifiers )
        : std::string( keyword )
        , usage( usage )
        , modifiers( modifiers )
        {

        }

        IOD::IOD( const std::string& keyword,
                  const std::string& name )
        : base()
        , keyword( keyword )
        , name( name )
        {

        }
    }
}
