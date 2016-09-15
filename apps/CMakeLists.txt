foreach( app iodc )
    add_executable( ${app} ${app} )
    target_link_libraries( ${app} ${Boost_PROGRAM_OPTIONS_LIBRARY} iodl_library )
endforeach()
