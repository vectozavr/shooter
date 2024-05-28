cmake_minimum_required(VERSION 3.17)
project(shooter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${CMAKE_PROJECT_NAME}
        Source.cpp
        player/Player.cpp
        player/Player.h
        network/ShooterClient.cpp
        network/ShooterClient.h
        network/ShooterServer.cpp
        network/ShooterServer.h
        weapon/Weapon.cpp
        weapon/Weapon.h
        weapon/Ak47.h
        weapon/Shotgun.h
        weapon/Gun.h
        weapon/Gold_Ak47.h
        weapon/Rifle.h
        player/PlayerController.cpp
        player/PlayerController.h
        Shooter.cpp
        Shooter.h
        ShooterConsts.h
        network/ShooterMsgType.h
        network/ShooterMsgType.cpp
        network/Chat.cpp
        network/Chat.h
)

add_executable(${CMAKE_PROJECT_NAME}_server
        Server.cpp
        player/Player.cpp
        player/Player.h
        network/ShooterServer.cpp
        network/ShooterServer.h
        ShooterConsts.h
        network/ShooterMsgType.h
        network/ShooterMsgType.cpp
        network/Chat.cpp
        network/Chat.h
        weapon/Weapon.cpp
        weapon/Weapon.h
)

# include 3dzavr engine into our project
add_subdirectory(3dzavr/engine)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 3DZAVR)
target_link_libraries(${CMAKE_PROJECT_NAME}_server PUBLIC 3DZAVR)
