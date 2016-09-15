#pragma once
#include <vector>
#include "iodl/ast/attribute.hpp"

namespace iodl
{
    namespace ast
    {
        /** A class to represent a set of DICOM attributes and constraints,
         *  i.e. DICOM Sequence Macros, IOD-Modules or Functional Groups.
         */
        class AttributeSet : public std::vector<Attribute>
        {
        public:
            /** An enumeration Type to discern the different kinds of
             *  attribute sets.
             */
            enum Flavor
            {
                MACRO,
                MODULE,
                FUNCTIONAL_GROUP
            };

            /** An enumeration type to provide different attribute set
             *  modifiers, currently only meaningful if
             *  flavor == FUNCTIONAL_GROUP.
             */
            enum Modifier
            {
                SHARED      = 1, // C++11: 0b0000001
                PER_FRAME   = 2  // C++11: 0b0000010
            };

            inline AttributeSet( Flavor flavor       = MACRO,
                                 std::string keyword = std::string(),
                                 std::string name    = std::string() )
            : flavor( flavor )
            , modifiers( 0 )
            , keyword( keyword )
            , name( name )
            {

            }

            /** Template parameters if the generated class should
             * be a class template. Otherwise empty.
             */
            std::vector<std::string> templateParameters;

            /// Discern of which kind this attribute set is, see Flavor.
            Flavor      flavor;

            /// A set of modifiers for this attribute set, see Modifiers.
            unsigned    modifiers;

            /** The identifier to use when generating code for this
             *  attribute set.
             */
            std::string keyword;

            /** The actual name of this attribute set, as given within the
             *  DICOM standard.
             */
            std::string name;
        };
    }
}
