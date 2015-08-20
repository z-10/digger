cmake_minimum_required(VERSION 3.1)

project(digger)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(SOURCE_FILES
main.c
digger.c
drawing.c
sprite.c
scores.c
record.c
sound.c
newsnd.c
ini.c
input.c
monster.c
bags.c
alpha.c
vgagrafx.c
title_gz.c
icon.c
sdl_kbd.c
sdl_vid.c
sdl_timer.c
sdl_snd.c
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
link_libraries(${LIBS} ${SDL2_LIBRARY} z objc)
set(SOURCE_FILES ${SOURCE_FILES} fbsd_sup.c)
add_definitions(-DDARWIN -D_SDL)

#todo : add windows


add_executable(digger ${SOURCE_FILES})
