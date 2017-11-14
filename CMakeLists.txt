PROJECT(noos)
cmake_minimum_required(VERSION 2.8)
set(LIBRARY_PATH ${LIBRARY_PATH}
		      /lib
		      /usr/lib
		      /usr/lib64
		      /usr/local/lib
		      /usr/local/lib64
		      /usr/lib/x86_64-linux-gnu)

include_directories(SYSTEM ${INCLUDE_HEADERS})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
endif()
add_executable(thread_pool example.cpp)
target_link_libraries(thread_pool ${Boost_LIBRARIES})
