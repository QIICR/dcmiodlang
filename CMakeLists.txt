cmake_minimum_required( VERSION 2.8.9 )

project( iod-language )

find_package( Boost 1.58.0 COMPONENTS program_options filesystem system REQUIRED )
find_package( DCMTK CONFIG REQUIRED )

include_directories( include ${Boost_INCLUDE_DIRS} ${DCMTK_INCLUDE_DIRS} )

foreach( dir libsrc apps )
    add_subdirectory( ${dir} )
endforeach()
