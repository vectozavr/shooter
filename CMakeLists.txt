cmake_minimum_required(VERSION 3.17)
project(shooter)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(shooter
        # game:
        Source.cpp
        Player.cpp
        Player.h
        Client.cpp
        Client.h
        Server.cpp
        Server.h
        weapon/Weapon.cpp
        weapon/Weapon.h
        weapon/Ak47.cpp
        weapon/Ak47.h
        weapon/Shotgun.cpp
        weapon/Shotgun.h
        weapon/Gun.cpp
        weapon/Gun.h
        Bonus.cpp
        Bonus.h
        weapon/Gold_Ak47.h
        weapon/Rifle.cpp
        weapon/Rifle.h
        PlayerController.cpp
        PlayerController.h
        Shooter.cpp
        Shooter.h
        # 3d engine:
        engine/utils/Time.h
        engine/utils/Time.cpp
        engine/utils/Point4D.h
        engine/utils/Point4D.cpp
        engine/utils/Matrix4x4.h
        engine/utils/Matrix4x4.cpp
        engine/Triangle.h
        engine/Triangle.cpp
        engine/Mesh.h
        engine/Mesh.cpp
        engine/utils/Log.h
        engine/utils/Log.cpp
        engine/ResourceManager.h
        engine/ResourceManager.cpp
        engine/World.h
        engine/World.cpp
        engine/Camera.h
        engine/Camera.cpp
        engine/Screen.h
        engine/Screen.cpp
        engine/Engine.h
        engine/Engine.cpp
        engine/Plane.h
        engine/Plane.cpp
        engine/Keyboard.cpp
        engine/Keyboard.h
        engine/Mouse.cpp
        engine/Mouse.h
        engine/animation/Animation.h
        engine/animation/Timeline.cpp
        engine/animation/Timeline.h
        engine/animation/Interpolation.h
        engine/animation/Animation.cpp
        engine/animation/ATranslate.h
        engine/animation/AScale.h
        engine/animation/ARotate.h
        engine/animation/AWait.h
        engine/physics/RigidBody.cpp
        engine/physics/RigidBody.h
        engine/physics/Simplex.h
        engine/physics/Solver.cpp
        engine/physics/Solver.h
        engine/Object.cpp
        engine/Object.h
        engine/gui/Button.cpp
        engine/gui/Button.h
        engine/gui/Window.cpp
        engine/gui/Window.h
        engine/network/ClientUDP.cpp
        engine/network/ClientUDP.h
        engine/network/MsgType.cpp
        engine/network/MsgType.h
        engine/network/ReliableMsg.cpp
        engine/network/ReliableMsg.h
        engine/network/ServerUDP.cpp
        engine/network/ServerUDP.h
        engine/network/UDPConnection.cpp
        engine/network/UDPConnection.h
        engine/network/UDPSocket.cpp
        engine/network/UDPSocket.h
        engine/network/config.h
        engine/animation/AFunction.h
        engine/Consts.h)

if(APPLE OR UNIX)
    include_directories(/usr/local/include)
else()
    set(SFML_DIR "SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()

target_link_libraries(shooter sfml-audio sfml-network sfml-graphics sfml-window sfml-system)