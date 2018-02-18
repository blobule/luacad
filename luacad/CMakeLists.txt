
cmake_minimum_required (VERSION 3.0)

project (luacadx)

find_package(Lua 5.3 REQUIRED)

set(LUA_LIBS_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib/lua/5.3)
 

install(FILES luacad.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
install(FILES matrix.lua complex.lua matrix-extra.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
install(FILES table-extra.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
install(FILES md5.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})
install(FILES cache.lua DESTINATION ${LUA_LIBS_INSTALL_PATH})




