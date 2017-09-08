cmake_minimum_required(VERSION 3.8)
project(WestWorldWithMessaging)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sources
        "*.cpp"
        "*.h"
        )


set(SOURCE_FILES ${sources})
add_executable(WestWorldWithMessaging ${SOURCE_FILES})