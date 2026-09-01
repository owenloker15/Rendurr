# Create static lib with only the cpp as its source
add_library(stb_image STATIC stb_image.cpp)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
