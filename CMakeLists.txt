cmake_minimum_required(VERSION 3.1)

project(digger)

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

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(SOURCE_FILES ${SOURCE_FILES} fbsd_sup.c)
find_package(SDL REQUIRED)
add_definitions(-D_SDL -DDARWIN -D_GNU_SOURCE=1 -D_THREAD_SAFE)
include_directories(${SDL_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL_LIBRARY})
link_libraries(${LIBS} z objc)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#todo : add windows


add_executable(digger ${SOURCE_FILES})
