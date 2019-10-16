cmake_minimum_required(VERSION 3.10)

project(scrapper)

set(CURL_LIBRARY "-lcurl") 

find_package(CURL REQUIRED) 

add_executable(scrapper main.c)

include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(scrapper ${CURL_LIBRARIES})