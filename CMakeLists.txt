project(bboxer)
cmake_minimum_required(VERSION 3.5)
add_compile_options(-std=c++11)

file(GLOB bboxer_SRC
        "*.h"
        "*.cpp")

set(CMAKE_INSTALL_PREFIX /usr/local/bin/bboxer)

find_package(OpenCV REQUIRED)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)


set(SOURCE_FILES main.cpp utils/file_traverser.cpp utils/testor.cpp utils/utils.h)
add_executable(bboxer ${bboxer_SRC} utils/file_traverser.cpp utils/testor.cpp utils/time.cpp
        utils/string_utils.cpp utils/print_utils.cpp)
target_link_libraries(bboxer ${CMAKE_DL_LIBS} ${OpenCV_LIBS})
