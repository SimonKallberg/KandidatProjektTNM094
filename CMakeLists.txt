cmake_minimum_required(VERSION "3.14.0")

project("dome_game")

add_executable("${PROJECT_NAME}" "main.cpp")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sgct)
target_include_directories("${PROJECT_NAME}" SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sgct/include)
target_link_libraries(
    "${PROJECT_NAME}"
    sgct_light glew glfw png16_static quat tinyxml2static turbojpeg-static
    vrpn
    ${GLFW_LIBRARIES}
)
