cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-MetaDataDB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-MetaDataDB main.cpp DataBase.cpp DataBase.h
        SQLController.cpp SQLController.h huffman_helper.cpp huffman_helper.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C json-c)
link_libraries(${JSON-C_LIBRARIES})
target_link_libraries(MyInvincibleLibrary-MetaDataDB ${JSON-C_LIBRARIES})