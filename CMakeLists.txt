cmake_minimum_required(VERSION 3.0.0)
project(VideoCapProc)
add_executable(VideoCapProc VideoCapProc.cpp)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
target_link_libraries(VideoCapProc opencv_core opencv_highgui opencv_video opencv_videoio opencv_imgproc)

