cmake_minimum_required(VERSION "3.13.0")

project("dome_game")

add_executable("${PROJECT_NAME}" "main.cpp")

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_11)

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sgct)
target_include_directories("${PROJECT_NAME}" SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sgct/include)
target_link_libraries(
    "${PROJECT_NAME}"
    sgct_light glew glfw png16_static quat tinyxml2static turbojpeg-static zlibstatic miniziplibstatic
    vrpn
    ${GLFW_LIBRARIES}
)

