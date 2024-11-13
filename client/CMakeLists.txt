cmake_minimum_required(VERSION 3.5)
project(Client)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(client main.cpp)
target_link_libraries(client Qt5::Widgets)
