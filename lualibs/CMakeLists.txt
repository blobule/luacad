
cmake_minimum_required (VERSION 3.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules/")

project (nef)

set (CMAKE_CXX_STANDARD 11)

OPTION(NEF_USE_OPENCSG "Use OpenCSG" ON)


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  
endif()

# include helper file
include( ${CGAL_USE_FILE})

# Boost and its components
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()

find_package( Eigen3 )
# EIGEN3_INCLUDE_DIR


set (NEF_SOURCES nef.cc nef2.cc
    cgalutils.cc hash.cc linalg.cc GeometryUtils.cc
    libtess2/Source/bucketalloc.c
    libtess2/Source/dict.c
    libtess2/Source/geom.c
    libtess2/Source/mesh.c
    libtess2/Source/priorityq.c
    libtess2/Source/sweep.c
    libtess2/Source/tess.c
    )

set(OCSG_LIBS "m")
set(OCSG_SOURCES "")

if( NEF_USE_OPENCSG )
    message(STATUS "Using OpenCSG")
    add_definitions(-DUSE_OPENCSG)

    find_package(OpenGL)
    if(NOT OPENGL_FOUND)
        message(STATUS "OPENGL needed.")
    else()
        message(STATUS "OpenGL found. ${OPENGL_LIBRARY} and ${OPENGL_INCLUDE_DIR}")
        set(OCSG_LIBS ${OCSG_LIBS} ${OPENGL_LIBRARY})
        set(OCSG_INCLUDE_DIR ${OCSG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
    endif()

    find_package(glfw3)
    if(NOT glfw3_FOUND)
        message(STATUS "GLFW3 needed.")
    else()
        message(STATUS "GLFW3 found. ${GLFW3_LIBRARY} and ${GLFW3_INCLUDE_DIR}")
        set(OCSG_LIBS ${OCSG_LIBS} ${GLFW3_LIBRARY})
        set(OCSG_INCLUDE_DIR ${OCSG_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR})
    endif()
    

    find_package(GLEW)
    if(NOT GLEW_FOUND)
        message(STATUS "GLEW needed.")
    else()
        message(STATUS "GLEW found. ${GLEW_LIBRARY} and ${GLEW_INCLUDE_DIR}")
        set(OCSG_LIBS ${OCSG_LIBS} ${GLEW_LIBRARY})
        set(OCSG_INCLUDE_DIR ${OCSG_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
    endif()

    find_package(OpenCSG)
    if(NOT OPENCSG_FOUND)
        message(STATUS "OPENCSG needed.")
    else()
        set(OCSG_LIBS ${OCSG_LIBS} ${OPENCSG_LIBS})
        set(OCSG_INCLUDE_DIR ${OCSG_INCLUDE_DIR} ${OPENCSG_INCLUDE_DIR})
    endif()

    message(STATUS "Finale OCSG LIBS is ${OCSG_LIBS}")
    message(STATUS "Finale OCSG INC  is ${OCSG_INCLUDE_DIR}")

    set(OCSG_SOURCES wrap_opencsg.cc
        ocsg.cc
        displaylistPrimitive.cpp
        ${NEF_SOURCES}
        )

endif()

### LFS
set(LFS_SOURCES lfs.c)
set(LFS_LIBS "")


find_package(Lua 5.3 REQUIRED)
message(STATUS "LUA ${LUA_LIBRARIES}")
 
include_directories (
    ${NEF_SOURCE_DIR} /usr/include/eigen3 libtess2/Include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LUA_INCLUDE_DIR}
    ${OCSG_INCLUDE_DIR}
    )
             

set (SOURCES wrap_nef.cc ${NEF_SOURCES} )

                     
add_library (${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${LUA_LIBRARIES})
set_target_properties (${PROJECT_NAME} PROPERTIES PREFIX "")
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION /usr/local/lib/lua/5.3)


if( NEF_USE_OPENCSG )
    add_library (opencsg SHARED ${OCSG_SOURCES})
    target_link_libraries (opencsg ${OCSG_LIBS}
        ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${LUA_LIBRARIES})
    set_target_properties (opencsg PROPERTIES PREFIX "")
    install(TARGETS opencsg LIBRARY DESTINATION /usr/local/lib/lua/5.3)
endif()

add_library (lfs SHARED ${LFS_SOURCES})
target_link_libraries (lfs lua5.3 ${LUA_LIBRARIES} ${LFS_LIBS} ${LUA_LIBRARIES})
set_target_properties (lfs PROPERTIES PREFIX "")
install(TARGETS lfs LIBRARY DESTINATION /usr/local/lib/lua/5.3)


#add_executable ( testglfw testglfw.c )
#target_link_libraries ( testglfw ${OCSG_LIBS} )



