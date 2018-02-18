cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules/")

#project (luacad)


add_subdirectory(lualibs)
add_subdirectory(tcllibs)
add_subdirectory(luacad)
add_subdirectory(editor)



