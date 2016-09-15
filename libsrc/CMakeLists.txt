foreach( component ast skeleton generator parser )
    add_subdirectory( ${component} )
    list( APPEND components $<TARGET_OBJECTS:${component}> )
endforeach()

add_library( iodl_library status ${components} )
set_target_properties( iodl_library PROPERTIES OUTPUT_NAME iodl )
target_link_libraries( iodl_library dcmdata oflog ofstd ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
