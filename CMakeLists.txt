cmake_minimum_required(VERSION 3.16)
# project(OpenGLlibrary)

file(GLOB srcfiles src/*.cpp)
file(COPY shaders DESTINATION .)
file(COPY textures DESTINATION .)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ext)

add_library(OpenGLlibrary ${srcfiles})


if(COMPILE_FOR_WIN)
    target_link_libraries(OpenGLlibrary glfw3 glad stb gdi32 sndfile OpenALcommon OpenAL32) # Windows
else()
    target_link_libraries(OpenGLlibrary glfw glad stb openal sndfile OpenALcommon)
endif()

target_include_directories(OpenGLlibrary PUBLIC include)
