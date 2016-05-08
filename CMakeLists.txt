cmake_minimum_required(VERSION 2.8)
#Déclaration du projet
cmake_policy(SET CMP0004 OLD)
project(Dactylo)
add_executable(Dactylo main.c)

include(CheckCCompilerFlag)
check_c_compiler_flag("-std=c99" C99_SUPPORTED)
if (C99_SUPPORTED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif()

set( CMAKE_C_FLAGS_DEBUG   " -ggdb3" )

#Déclaration de l'exécutable

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries (Dactylo ${OPENGL_LIBRARIES})
endif (OPENGL_FOUND)

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
  include_directories(${GLUT_INCLUDE_DIR})
  target_link_libraries (Dactylo ${GLUT_LIBRARIES})
endif (GLUT_FOUND)

