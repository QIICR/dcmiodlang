#pragma once

namespace iodl
{
    namespace ast
    {
        struct ValueMultiplicity
        {
            inline ValueMultiplicity( unsigned min = 0,
                                     unsigned max = 0,
                                     unsigned increment = 1 )
            : min( min )
            , max( max )
            , increment( increment )
            {

            }

            unsigned min;
            unsigned max;
            unsigned increment;
        };
    }
}
