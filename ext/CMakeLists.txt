add_library(glad src/gl.cpp)
target_include_directories(glad PUBLIC include)

add_library(stb src/stb.cpp)
target_include_directories(stb PUBLIC include)

add_subdirectory(OpenALcommon-git)