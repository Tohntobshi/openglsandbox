cmake_minimum_required(VERSION 2.8)
project( HelloGL )

set(
  TARGETS
    src/main.cpp src/gameWindow.cpp src/physicalModel.cpp src/physicalWorld.cpp src/glErrChecks.cpp src/shader.cpp
    src/visualModel.cpp src/camera.cpp
)

include_directories(libs/imgui/include)

add_executable( hellogl ${TARGETS} )

add_subdirectory( libs/imgui )

target_link_libraries(hellogl imgui)
target_link_libraries(hellogl SDL2)
target_link_libraries(hellogl SOIL)
target_link_libraries(hellogl GLESv2)
target_link_libraries(hellogl pthread)


