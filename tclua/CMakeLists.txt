
cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules/")

project (tclua)


find_package( Lua REQUIRED )
#if( NOT CGAL_FOUND )
#    message(STATUS "Lua?")
#endif()

find_package( TCL REQUIRED )

message(STATUS "tcl ${TCL_LIBRARY} ${TCL_INCLUDE_PATH}")
message(STATUS "tk ${TK_LIBRARY} ${TK_INCLUDE_PATH}")
message(STATUS "lua ${LUA_INCLUDE_DIR} : ${LUA_LIBRARY} : ${LUA_MATH_LIBRARY}")

#########################

add_library(hello SHARED hello.c)
target_include_directories (hello PUBLIC
    ${TCL_INCLUDE_PATH}
    ${TK_INCLUDE_PATH}
    ${LUA_INCLUDE_DIR}
    )
target_link_libraries (hello
    ${TCL_LIBRARY}
    ${TK_LIBRARY}
    ${LUA_LIBRARY} ${LUA_MATH_LIBRARY}
    )

install(DIRECTORY "hello" DESTINATION "/usr/local/lib/tcltk/" DIRECTORY_PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(TARGETS hello LIBRARY DESTINATION /usr/local/lib/tcltk/hello)

############################

add_library(tclua SHARED tclua.c)
target_include_directories (tclua PUBLIC
    ${TCL_INCLUDE_PATH}
    ${TK_INCLUDE_PATH}
    ${LUA_INCLUDE_DIR}
    )
target_link_libraries (tclua
    ${TCL_LIBRARY}
    ${TK_LIBRARY}
    ${LUA_LIBRARY} ${LUA_MATH_LIBRARY}
    )

install(DIRECTORY "tclua" DESTINATION "/usr/local/lib/tcltk/" DIRECTORY_PERMISSIONS
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(TARGETS tclua LIBRARY DESTINATION /usr/local/lib/tcltk/tclua)








