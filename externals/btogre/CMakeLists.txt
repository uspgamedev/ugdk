cmake_minimum_required(VERSION 2.8.0)

project(BtOgre)

add_library(BtOgre STATIC BtOgre.cpp)
target_link_libraries(BtOgre LINK_PUBLIC ${BULLET_LIBRARIES} ${OGRE_LIBRARIES})
target_include_directories(BtOgre
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/ 
    PUBLIC ${BULLET_INCLUDE_DIRS}
    PUBLIC ${OGRE_INCLUDE_DIRS} )
