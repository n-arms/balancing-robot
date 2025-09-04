cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Name of your project (also the default firmware filename)
project(balancing_robot)

# Force modern C++ for everything
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # enables gnu++20 instead of -std=c++20
