project(imgui)

add_library(${PROJECT_NAME} STATIC
	imgui.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui_demo.cpp
	imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 17
)
