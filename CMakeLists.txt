cmake_minimum_required(VERSION 3.16)
project(ca_engine)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

#target_include_directories(ca_engine PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           )

file(GLOB SOURCES "*.cpp")

# Create your game executable target as usual
add_executable(ca_engine WIN32 testing.cpp sample_rules.cpp evo_rule.cpp langtons_ant.cpp)

# Designate the console as the target (remove when making final app)
# set_target_properties(ca_engine PROPERTIES LINK_SUBSYSTEM CONSOLE)

# Link to the actual SDL3 library.
target_link_libraries(ca_engine PRIVATE SDL3::SDL3)