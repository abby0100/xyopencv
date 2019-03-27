
#message("\n[+] cmake")
cmake_minimum_required(VERSION 2.8)

#message("\n[+] project")
project(xy-project)

add_definitions(-std=c++1y)

#message("\n[+] packages")
find_package(OpenCV REQUIRED)
#message("	version: ${OpenCV_VERSION}")
#message("	library: ${OpenCV_LIBS}")
#message("	include: ${OpenCV_INCLUDE_DIRS}")

#if(CMAKE_VERSION VERSION_LESS "2.8.11")
#  include_directories(${OpenCV_INCLUDE_DIRS})
#endif()

add_executable(opencv-xy main.cpp TimeUtil.cpp)

target_link_libraries(opencv-xy ${OpenCV_LIBS})
target_link_libraries(opencv-xy pthread)
#target_link_libraries(opencv-xy X11)
