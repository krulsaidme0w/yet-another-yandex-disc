project(yet_another_yandex_disc CXX)
set(CMAKE_CXX_STANDARD 20)

CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

set(CMAKE_CXX_FLAGS         "-Wno-unused -Wno-deprecated -Wno-comment -Wno-reorder -Wno-unused-parameter -Wno-unused-result -fpermissive -lpthread -pthread")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(vendor)
add_subdirectory(cmd)
