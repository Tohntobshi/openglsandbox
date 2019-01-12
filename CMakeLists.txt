cmake_minimum_required(VERSION 2.8)
project( HelloGL )

set(TARGETS main.cpp gameWindow.cpp model.cpp world.cpp glErrChecks.cpp shader.cpp)

add_executable( hellogl ${TARGETS} )

target_link_libraries(hellogl SDL2)
target_link_libraries(hellogl GLESv2)
target_link_libraries(hellogl pthread)


