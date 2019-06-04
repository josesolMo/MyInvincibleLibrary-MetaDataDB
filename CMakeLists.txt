cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-MetaDataDB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-MetaDataDB main.cpp
        DBController.cpp DBController.h)