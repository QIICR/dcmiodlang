#include "iodl/ast/attribute.hpp"
#include "dcmtk/dcmdata/dcdict.h"

namespace iodl
{
    namespace ast
    {
        Attribute::Attribute( const DcmTagKey& tag,
                              std::string keyword,
                              Type type,
                              std::string text )
        : tag( tag )
        , keyword( keyword )
        , arguments()
        , type( type )
        , text( text )
        , dictionaryEntry( NULL )
        {

        }

        Attribute::LookupStatus Attribute::lookupInDictionary()
        {
            if( dictionaryEntry = dcmDataDict.rdlock().findEntry( tag, NULL ) )
            {
                if( keyword != dictionaryEntry->getTagName() )
                {
                    dictionaryEntry = NULL;
                    return TAG_MISMATCH;
                }
                return OK;
            }
            return NOT_FOUND;
        }
    }
}
