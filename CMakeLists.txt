project(dome_game)


add_executable(dome_game main.cpp)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sgct)
target_include_directories(dome_game SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sgct/include)
target_link_libraries(
    dome_game
    sgct_light glew glfw png16_static quat tinyxml2static turbojpeg-static
    vrpn
    ${GLFW_LIBRARIES}
)
