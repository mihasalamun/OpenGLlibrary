file(GLOB srcfiles src/*.c*)
add_library(OpenALcommon ${srcfiles})
target_include_directories(OpenALcommon PUBLIC include)
