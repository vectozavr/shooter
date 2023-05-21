cmake_minimum_required(VERSION 3.17)
project(shooter)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${CMAKE_PROJECT_NAME}
        # game:
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
        # 3d engine:
        3dzavr/engine/Consts.h
        3dzavr/engine/math/Vec4D.h
        3dzavr/engine/math/Vec4D.cpp
        3dzavr/engine/math/Vec3D.cpp
        3dzavr/engine/math/Vec3D.h
        3dzavr/engine/math/Vec2D.cpp
        3dzavr/engine/math/Vec2D.h
        3dzavr/engine/math/Matrix4x4.h
        3dzavr/engine/math/Matrix4x4.cpp
        3dzavr/engine/Triangle.h
        3dzavr/engine/Triangle.cpp
        3dzavr/engine/math/Plane.h
        3dzavr/engine/math/Plane.cpp
        3dzavr/engine/Mesh.h
        3dzavr/engine/Mesh.cpp
        3dzavr/engine/utils/Log.h
        3dzavr/engine/utils/Log.cpp
        3dzavr/engine/utils/Time.h
        3dzavr/engine/utils/Time.cpp
        3dzavr/engine/utils/Timer.cpp
        3dzavr/engine/utils/Timer.h
        3dzavr/engine/utils/ResourceManager.h
        3dzavr/engine/utils/ResourceManager.cpp
        3dzavr/engine/World.h
        3dzavr/engine/World.cpp
        3dzavr/engine/Camera.h
        3dzavr/engine/Camera.cpp
        3dzavr/engine/io/Screen.h
        3dzavr/engine/io/Screen.cpp
        3dzavr/engine/Engine.h
        3dzavr/engine/Engine.cpp
        3dzavr/engine/io/Keyboard.cpp
        3dzavr/engine/io/Keyboard.h
        3dzavr/engine/io/Mouse.cpp
        3dzavr/engine/io/Mouse.h
        3dzavr/engine/io/SoundController.cpp
        3dzavr/engine/io/SoundController.h
        3dzavr/engine/utils/ObjectController.cpp
        3dzavr/engine/utils/ObjectController.h
        3dzavr/engine/animation/Animation.h
        3dzavr/engine/animation/Timeline.cpp
        3dzavr/engine/animation/Timeline.h
        3dzavr/engine/animation/Interpolation.h
        3dzavr/engine/animation/Animation.cpp
        3dzavr/engine/animation/ATranslate.h
        3dzavr/engine/animation/AScale.h
        3dzavr/engine/animation/ARotate.h
        3dzavr/engine/animation/AWait.h
        3dzavr/engine/animation/AFunction.h
        3dzavr/engine/animation/AAttractToPoint.h
        3dzavr/engine/animation/ARotateRelativePoint.h
        3dzavr/engine/animation/ARotateLeft.h
        3dzavr/engine/animation/Interpolation.cpp
        3dzavr/engine/animation/Animations.h
        3dzavr/engine/animation/AShowCreation.h
        3dzavr/engine/animation/AShowUncreation.h
        3dzavr/engine/animation/ARotateLeftUpLookAt.h
        3dzavr/engine/animation/ADecompose.h
        3dzavr/engine/physics/RigidBody.cpp
        3dzavr/engine/physics/RigidBody.h
        3dzavr/engine/physics/Simplex.h
        3dzavr/engine/physics/HitBox.cpp
        3dzavr/engine/physics/HitBox.h
        3dzavr/engine/Object.cpp
        3dzavr/engine/Object.h
        3dzavr/engine/gui/Button.cpp
        3dzavr/engine/gui/Button.h
        3dzavr/engine/gui/Window.cpp
        3dzavr/engine/gui/Window.h
        3dzavr/engine/network/ClientUDP.cpp
        3dzavr/engine/network/ClientUDP.h
        3dzavr/engine/network/MsgType.cpp
        3dzavr/engine/network/MsgType.h
        3dzavr/engine/network/ReliableMsg.cpp
        3dzavr/engine/network/ReliableMsg.h
        3dzavr/engine/network/ServerUDP.cpp
        3dzavr/engine/network/ServerUDP.h
        3dzavr/engine/network/UDPConnection.cpp
        3dzavr/engine/network/UDPConnection.h
        3dzavr/engine/network/UDPSocket.cpp
        3dzavr/engine/network/UDPSocket.h
        )

if(APPLE OR UNIX)
    include_directories(/usr/local/include)
else()
    set(SFML_DIR "C:/Libraries/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} sfml-audio sfml-network sfml-graphics sfml-window sfml-system)

# OpenGL part
if (APPLE)
    set(GLEW_H /usr/local/Cellar/glew/2.1.0/include/GL)
    set(GLFW_H /usr/local/Cellar/glfw/3.2.1/include/GLFW)
    include_directories(${GLEW_H} ${GLFW_H})

    set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
    set(GLFW_LINK /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.dylib)
    link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework GLUT")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)

    target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${GLUT_LIBRARY})
endif()
