cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Bezier-x86)
enable_language(ASM_NASM)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(LIBS ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}  ${CMAKE_DL_LIBS} m)

add_executable(bezier bezier.asm main.c)
target_link_libraries(bezier ${LIBS})
