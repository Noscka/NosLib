project ("NosLibTesting" VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosLib")