cmake_minimum_required(VERSION 3.10)

project(scrapper)

set(CURL_LIBRARY "-lcurl") 
set(PTHREAD_LIBRARY "-lpthread")
set (HEADERS headers/structures.h)
set(SRCS src/scrapper.c src/structures.c)
# set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/headers)
# set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# foreach(h ${HEADERS})
#     list(APPEND HEADER_LIST ${INCLUDE_DIR}/${h}.h)
# endforeach()

# foreach(s ${SRCS})
#     list(APPEND SOURCE_LIST ${SRC_DIR}/${s}.c)
# endforeach()

# set(TGT scrapper)

# include_directories(${INCLUDE_DIR})


set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)
# target_link_libraries(my_app Threads::Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)

find_package(CURL REQUIRED) 

add_executable(scrapper ${SRCS} ${HEADERS})

include_directories(${CURL_INCLUDE_DIR})



target_link_libraries(scrapper ${CURL_LIBRARIES} Threads::Threads)