cmake_minimum_required(VERSION "3.13.0")

project("dome_game")

add_executable("${PROJECT_NAME}"
"src/main.cpp"
"src/ServerHandler.hpp"
"src/boxtest.hpp"
"src/Quad.hpp"
"src/Quad.cpp"
"src/Scene.hpp"
"src/Scene.cpp"
"src/DomeGame.hpp"
"src/DomeGame.cpp"
"src/ModelLoader.hpp"
"src/ModelLoader.cpp"
"src/DomeDrawable.hpp"
"src/DomeDrawable.cpp"
"src/DomeMovable.hpp"
"src/DomeMovable.cpp"
"src/Player.hpp"
"src/Player.cpp"
"src/Weapon.hpp"
"src/Weapon.cpp"
"src/Projectile.cpp"
"src/Projectile.hpp"
)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_11)

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)

include_directories(${SGCT_INCLUDE_DIRECTORY} libwebsockets/include)
add_subdirectory(libwebsockets)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sgct)

target_include_directories("${PROJECT_NAME}" SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sgct/include)
target_link_libraries(
    "${PROJECT_NAME}"
    sgct_light glew glfw png16_static quat tinyxml2static turbojpeg-static zlibstatic miniziplibstatic
    vrpn freetype
    ${GLFW_LIBRARIES}
    websockets
)


if( MSVC )
	set(LIBS
		${SGCT_LIBS}
		${OPENGL_gl_LIBRARY}
		websockets
		ws2_32
	)
elseif( WIN32 ) #MINGW or similar
	set(LIBS
		${SGCT_LIBS}
		${OPENGL_gl_LIBRARY}
		websockets
		ws2_32
	)
elseif( APPLE )
	find_library(COCOA_LIBRARY Cocoa REQUIRED)
	find_library(IOKIT_LIBRARY IOKit REQUIRED)
	find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

	set(LIBS
		${SGCT_LIBS}
		${OPENGL_gl_LIBRARY}
		${COCOA_LIBRARY}
		${IOKIT_LIBRARY}
		${COREVIDEO_LIBRARY}
		websockets
	)
	else() #linux
		find_package(X11 REQUIRED)
		find_package(Threads REQUIRED)

		set(LIBS
			${SGCT_LIBS}
			${OPENGL_gl_LIBRARY}
			${X11_X11_LIB}
			${X11_Xrandr_LIB}
			${X11_Xinerama_LIB}
			${X11_Xinput_LIB}
			${X11_Xxf86vm_LIB}
			${X11_Xcursor_LIB}
			${CMAKE_THREAD_LIBS_INIT}
			websockets
		)
	endif()
