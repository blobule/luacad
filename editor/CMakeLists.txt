
cmake_minimum_required (VERSION 3.0)

project (editor)

find_package(TCL 8.6 REQUIRED)
find_package(Lua 5.3 REQUIRED)



#install(FILES luacad.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
#install(FILES matrix.lua complex.lua matrix-extra.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
#install(FILES table-extra.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
#install(FILES md5.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
#install(FILES cache.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})


install(PROGRAMS luacad-editor.tcl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES editor-csg.lua DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lua/5.3)

install(PROGRAMS dohelp.tcl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/tcltk/dohelp)
install(FILES dohelp-pkgIndex.tcl RENAME pkgIndex.tcl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/tcltk/dohelp)

install(DIRECTORY docs DESTINATION /usr/local/share/luacad USE_SOURCE_PERMISSIONS)


