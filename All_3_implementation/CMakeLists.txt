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

add_executable(server ../src/server.cpp)
target_link_libraries(server vsomeip3 ${Boost_LIBRARIES})

add_executable(client ../src/client.cpp)
target_link_libraries(client vsomeip3 ${Boost_LIBRARIES})


