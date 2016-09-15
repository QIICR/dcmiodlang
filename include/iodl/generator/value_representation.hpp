#pragma once
#include "dcmtk/dcmdata/dcvr.h"
#include "iodl/generator/expression.hpp"

namespace iodl
{
    namespace generator
    {
        class ValueRepresentation
        : public Boolean
        {
            struct vr_lut;
        public:
            static Component VR;

            ValueRepresentation( DcmEVR evr = EVR_UNKNOWN );

            virtual Component findComponent( const std::string& name );
            virtual Status generate( Context& context );
            virtual std::pair<boost::shared_ptr<Component::Base>,Status>
            compare( const Comparable& rhs );
            virtual bool result() const;

            DcmEVR evr;
        };
    }
}
