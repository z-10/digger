cmake_minimum_required(VERSION 3.1)

project(digger)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(SOURCE_FILES
main.cpp
digger.cpp
drawing.cpp
sprite.cpp
scores.cpp
record.cpp
sdl_timer.cpp
sdl_snd.cpp
sound.cpp
newsnd.cpp
ini.cpp
input.cpp
monster.cpp
bags.cpp
alpha.cpp
vgagrafx.cpp
title_gz.cpp
icon.cpp
sdl_kbd.cpp
sdl_vid.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})
link_libraries(${LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} z objc)
set(SOURCE_FILES ${SOURCE_FILES} fbsd_sup.cpp)

#todo : add windows


add_executable(digger ${SOURCE_FILES})
