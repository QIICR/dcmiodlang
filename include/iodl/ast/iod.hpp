#pragma once
#include "iodl/ast/attribute_set.hpp"

namespace iodl
{
    namespace ast
    {
        struct IOD
        {
            /** An enumeration type to provide different attribute set
             *  modifiers, currently only meaningful for functional
             *  groups.
             */
            enum Modifier
            {
                SHARED      = 1, // C++11: 0b0000001
                PER_FRAME   = 2  // C++11: 0b0000010
            };

            enum Usage
            {
                MANDATORY,
                CONDITIONAL,
                USER_OPTION
            };

            struct ModuleReference : std::string
            {
                ModuleReference( const std::string& keyword = std::string(),
                                 Usage usage = MANDATORY );
                Usage usage;
            };

            struct FunctionalGroupReference : std::string
            {
                FunctionalGroupReference( const std::string& keyword = std::string(),
                                          Usage usage = MANDATORY,
                                          unsigned modifiers = 0 );
                Usage usage;
                unsigned modifiers;
            };

            IOD( const std::string& keyword = std::string(),
                 const std::string& name = std::string() );

            /** Template parameters if the generated class should
             * be a class template. Otherwise empty.
             */
            std::vector<std::string> templateParameters;

            /// TODO
            std::string base;

            /** The identifier to use when generating code for this
             *  IOD.
             */
            std::string keyword;

            /** The actual name of this IOD, as given within the
             *  DICOM standard.
             */
            std::string name;

            /**
             */
            std::vector<ModuleReference> modules;

            /**
             */
            std::vector<FunctionalGroupReference> functionalGroups;
        };
    }
}
