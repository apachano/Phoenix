project (ImGui)

add_library(${PROJECT_NAME}
	imconfig.h
	imgui.h
	imgui_internal.h
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h

	imgui.cpp
	imgui_draw.cpp
	imgui_demo.cpp
	imgui_widgets.cpp

	examples/imgui_impl_opengl3.h
	examples/imgui_impl_opengl3.cpp

	examples/imgui_impl_sdl.h
	examples/imgui_impl_sdl.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/../Glad/include
	${CMAKE_CURRENT_LIST_DIR}/../SDL2/include
)

target_link_libraries(${PROJECT_NAME} Glad SDL2-static)
