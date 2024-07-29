cmake_minimum_required (VERSION 3.13)
project(vsomeip_10_min)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(ser ../src/ser.cpp)
target_link_libraries(ser vsomeip3 ${Boost_LIBRARIES})

add_executable(cli ../src/cli.cpp)
target_link_libraries(cli vsomeip3 ${Boost_LIBRARIES})

add_executable(res ../src/res.cpp)
target_link_libraries(res vsomeip3 ${Boost_LIBRARIES})

add_executable(req ../src/req.cpp)
target_link_libraries(req vsomeip3 ${Boost_LIBRARIES})

add_executable(pub ../src/pub.cpp)
target_link_libraries(pub vsomeip3 ${Boost_LIBRARIES})

add_executable(sub ../src/sub.cpp)
target_link_libraries(sub vsomeip3 ${Boost_LIBRARIES})
