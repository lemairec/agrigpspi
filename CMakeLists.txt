cmake_minimum_required(VERSION 2.8)
project(gps_agri)

configure_file (
"${PROJECT_SOURCE_DIR}/src/environnement.hpp.in"
"${PROJECT_SOURCE_DIR}/src/environnement.hpp"
)


FIND_PACKAGE(Qt5Core)
set(CMAKE_AUTOMOC ON)

find_package(Boost REQUIRED COMPONENTS system)
include_directories( ${Boost_INCLUDE_DIR} )



FILE(GLOB_RECURSE src_hpp src/*.hpp)
FILE(GLOB_RECURSE src_cpp src/*.cpp)

set(sources ${src_hpp} ${src_cpp} src/environnement.hpp.in)

FIND_PACKAGE(Qt5Core)
add_executable(gps_agri ${sources})
qt5_use_modules(gps_agri Widgets)

target_link_libraries(gps_agri ${Boost_LIBRARIES})

###
# grouping files for visual studio or xcode
###

SET(dirlist "")
FOREACH(file_path ${sources})
    GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
    STRING(REPLACE "${PROJECT_SOURCE_DIR}/" "" dir_path2 ${dir_path})
    SET(dir_list ${dir_list} ${dir_path2})
ENDFOREACH()
LIST(REMOVE_DUPLICATES dir_list)

foreach(next_ITEM ${dir_list})
    file(GLOB   source_ExecutionProvider_source ${next_ITEM}/*)
    STRING(REPLACE "/" "\\\\" source_group_path ${next_ITEM})
    source_group(${source_group_path} FILES ${source_ExecutionProvider_source})
endforeach(next_ITEM ${MY_SOURCES})
