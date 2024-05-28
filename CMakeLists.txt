cmake_minimum_required(VERSION 3.27)
project(search_engine)

set(CMAKE_CXX_STANDARD 23)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


set(MY_INCLUDE_DIR${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)


FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

add_subdirectory(src)

add_subdirectory(tests)