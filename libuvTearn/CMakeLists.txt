add_definitions(-std=c++11)
PROJECT(libuvMain)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

include_directories(/root/leefile/git/QtLearn/libuvTearn/libuv/include)
link_directories(/root/leefile/git/QtLearn/libuvTearn/libuv/lib)

# include_directories(/root/leefile/git/QtLearn/libuvTearn/soTest)
# link_directories(/root/leefile/git/QtLearn/libuvTearn/soTest)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(libuvMain ${DIR_SRCS})

TARGET_LINK_LIBRARIES(libuvMain uv)
