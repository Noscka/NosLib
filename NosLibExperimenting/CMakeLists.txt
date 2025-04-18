project ("NosLibExperimenting" VERSION 1.0.0 LANGUAGES CXX)

# This is just for me to experiment while I develop new library features

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosLib")

Set_Harsh_Compiler_flags(${PROJECT_NAME})

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
	COMMAND_EXPAND_LISTS
)