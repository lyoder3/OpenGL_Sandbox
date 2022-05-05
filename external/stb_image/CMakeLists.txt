cmake_minimum_required(VERSION 3.16.3)

project(STB_Image)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(${PROJECT_NAME} STATIC stb_image.cpp stb_image.h)
