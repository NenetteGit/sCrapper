cmake_minimum_required(VERSION 3.10)

project(scrapper)

set(CURL_LIBRARY "-lcurl") 

set(PTHREAD_LIBRARY "-lpthread")

set (HEADERS headers/parser.h headers/runner.h)

set(SRCS src/scrapper.c src/parser.c src/runner.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

find_package(CURL REQUIRED) 

add_executable(scrapper ${SRCS} ${HEADERS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CURL_INCLUDE_DIR})

target_compile_options(scrapper PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(scrapper ${CURL_LIBRARIES} Threads::Threads)