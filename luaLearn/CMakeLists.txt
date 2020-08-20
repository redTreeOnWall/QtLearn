# add_definitions(-std=c++11)
PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#ADD_SUBDIRECTORY( src )
AUX_SOURCE_DIRECTORY(. DIR_SRCS)


message("xxxxx")  
message(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

ADD_EXECUTABLE(main ${DIR_SRCS})
TARGET_LINK_LIBRARIES(main lua_static_5.3.3 m)
