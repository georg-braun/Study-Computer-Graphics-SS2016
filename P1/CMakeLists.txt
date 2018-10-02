cmake_minimum_required(VERSION 2.8)

# Project name
set(PROJECTNAME P1)
project(${PROJECTNAME})

# Define header and source files
set(HEADERS ) # add more header files here, seperated by spaces
set(SOURCES main.cpp) # add more source files here, seperated by spaces

# Set DEBUG/RELEASE
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)


# Find (free)GLUT
if (WIN32)
    # There should be a nicer way to do this...
    set(GLUT_LIBRARY "freeglut")
    set(GLUT_FOUND TRUE) # This might be a lie
else (WIN32)
    # Locating GLUT/freeGLUT on Unix/Linux
    find_package(GLUT)
    include_directories(${GLUT_INCLUDE_DIRS})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})
endif (WIN32)
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)


# Find OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


# Add headers and sources to project
add_executable(${PROJECTNAME} ${HEADERS} ${SOURCES})

# Linking
target_link_libraries(${PROJECTNAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})



