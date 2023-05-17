cmake_minimum_required(VERSION 3.9)
project(Tanks2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include_directories("/usr/include/SDL")

add_executable(Tanks2
        images/Brick.bmp
        images/Bricks.bmp
        images/deadTankSprite.bmp
        images/ExplosionSprite.bmp
        images/ReadMe.rtf
        images/ReadMe.txt
        images/TankSpriteBlue.bmp
        images/TankSpriteRed.bmp
        images/Tree1.bmp
        ItemList.cpp
        ItemList.h
        main.cpp DrawText.cpp DrawText.h gameMessageBox.cpp gameMessageBox.h)

target_link_libraries(Tanks2
        "/usr/lib/x86_64-linux-gnu/libSDL.so"
        "/usr/lib/x86_64-linux-gnu/libSDL_ttf.so"
        "/usr/lib/x86_64-linux-gnu/libSDL_mixer.so")

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/images
        ${CMAKE_BINARY_DIR}/images)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/fonts
        ${CMAKE_BINARY_DIR}/fonts)