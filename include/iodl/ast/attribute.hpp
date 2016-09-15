#pragma once
#include <vector>
#include <boost/variant.hpp>
#include "iodl/ast/attribute.hpp"
#include "iodl/ast/value_multiplicity.hpp"
#include "iodl/ast/value_set_constraint.hpp"
#include "dcmtk/dcmdata/dcdicent.h"
#include "dcmtk/dcmdata/dctagkey.h"

namespace iodl
{
    namespace ast
    {
        /** A class to represent a reference DICOM attributes and defining
         *  constraints for their occurrence.
         */
        class Attribute : public std::vector<Attribute>
        {
        public:
            /** An enumeration type to specify the type of an attribute, e. g.
             *  1C or 3.
             */
            enum Type
            {
                type1,
                type1C,
                type2,
                type2C,
                type3
            };

            enum LookupStatus
            {
                OK,
                NOT_FOUND,
                TAG_MISMATCH
            };

            struct MacroReference : std::string
            {
                inline MacroReference()
                : std::string()
                {

                }

                inline MacroReference( const std::string& keyword )
                : std::string( keyword )
                {

                }
            };

            typedef boost::variant
            <
                MacroReference,
                ValueMultiplicity,
                ValueSetConstraint
            > Argument;

            Attribute( const DcmTagKey& tag = DcmTagKey(),
                       std::string keyword  = std::string(),
                       Type type            = type1,
                       std::string text     = std::string() );

            LookupStatus lookupInDictionary();

            /** The unique Tag value of the attribute.
             */
            DcmTagKey tag;

            /** The keyword.
             */
            std::string keyword;

            /** Additional arguments for the attribute, e.g. which sequence macro
             *  to use for sequence attributes.
             */
            std::vector<Argument> arguments;

            /// The type of this attribute, see Type.
            Type type;

            /** A user defined free text string, e.g. the condition specified
             *  for a type 1C attribute.
             */
            std::string text;

            /** TODO
             */
            const DcmDictEntry* dictionaryEntry;
        };
    }
}
