cmake_minimum_required(VERSION 3.20)
project(ResolutionEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Опции сборки
option(BUILD_EDITOR "Build with editor support" ON)
option(BUILD_TESTS "Build tests" OFF)

# Поиск зависимостей
find_package(OpenGL REQUIRED)
find_package(GLUT QUIET)

# Если GLUT не найден, используем GLFW
if(NOT GLUT_FOUND)
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        message(STATUS "GLFW not found, will use bundled dependencies")
    endif()
endif()

# Включаем директорию include
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
)

# Исходные файлы ядра
set(CORE_SOURCES
    src/Core/Engine.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp
    src/Core/Timer.cpp
    src/Core/Logger.cpp
    src/Core/Config.cpp
)

# Исходные файлы рендерера
set(RENDERER_SOURCES
    src/Renderer/Renderer.cpp
    src/Renderer/Camera.cpp
    src/Renderer/Mesh.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture.cpp
    src/Renderer/Material.cpp
    src/Renderer/Light.cpp
    src/Renderer/Skybox.cpp
    src/Renderer/Fog.cpp
    src/Renderer/ModelLoader.cpp
)

# Исходные файлы физики
set(PHYSICS_SOURCES
    src/Physics/PhysicsWorld.cpp
    src/Physics/Collider.cpp
    src/Physics/Raycast.cpp
    src/Physics/Trigger.cpp
    src/Physics/Gravity.cpp
)

# Исходные файлы скриптинга
set(SCRIPTING_SOURCES
    src/Scripting/LuaVM.cpp
    src/Scripting/LuaBindings.cpp
    src/Scripting/ScriptManager.cpp
)

# Исходные файлы сущностей
set(ENTITIES_SOURCES
    src/Entities/Entity.cpp
    src/Entities/EntityManager.cpp
    src/Entities/Player.cpp
    src/Entities/NPC.cpp
    src/Entities/Item.cpp
    src/Entities/Door.cpp
    src/Entities/Prop.cpp
    src/Entities/Decal.cpp
)

# Исходные файлы ИИ
set(AI_SOURCES
    src/AI/AIController.cpp
    src/AI/Pathfinding.cpp
    src/AI/BehaviorTree.cpp
    src/AI/Squad.cpp
)

# Исходные файлы оружия
set(WEAPONS_SOURCES
    src/Weapons/Weapon.cpp
    src/Weapons/WeaponManager.cpp
    src/Weapons/Projectile.cpp
    src/Weapons/HitScan.cpp
)

# Исходные файлы UI
set(UI_SOURCES
    src/UI/UIManager.cpp
    src/UI/HUD.cpp
    src/UI/Crosshair.cpp
    src/UI/HealthBar.cpp
    src/UI/AmmoCounter.cpp
)

# Исходные файлы аудио
set(AUDIO_SOURCES
    src/Audio/AudioManager.cpp
    src/Audio/SoundSource.cpp
    src/Audio/SoundListener.cpp
    src/Audio/SoundBuffer.cpp
)

# Исходные файлы редактора
set(EDITOR_SOURCES
    src/Editor/Editor.cpp
    src/Editor/Viewport.cpp
    src/Editor/Hierarchy.cpp
    src/Editor/Properties.cpp
    src/Editor/BrushTools.cpp
    src/Editor/AssetBrowser.cpp
    src/Editor/MapSerializer.cpp
)

# Исходные файлы ассетов
set(ASSETS_SOURCES
    src/Assets/AssetManager.cpp
    src/Assets/AssetImporter.cpp
    src/Assets/TextureLoader.cpp
    src/Assets/ModelImporter.cpp
)

# Собираем все исходники
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${RENDERER_SOURCES}
    ${PHYSICS_SOURCES}
    ${SCRIPTING_SOURCES}
    ${ENTITIES_SOURCES}
    ${AI_SOURCES}
    ${WEAPONS_SOURCES}
    ${UI_SOURCES}
    ${AUDIO_SOURCES}
    ${EDITOR_SOURCES}
    ${ASSETS_SOURCES}
)

# Заголовочные файлы
set(ALL_HEADERS
    include/Core/Engine.h
    include/Core/Window.h
    include/Core/Input.h
    include/Core/Timer.h
    include/Core/Logger.h
    include/Core/Config.h
    include/Core/Types.h
    include/Core/Math.h
    
    include/Renderer/Renderer.h
    include/Renderer/Camera.h
    include/Renderer/Mesh.h
    include/Renderer/Shader.h
    include/Renderer/Texture.h
    include/Renderer/Material.h
    include/Renderer/Light.h
    include/Renderer/Skybox.h
    include/Renderer/Fog.h
    include/Renderer/ModelLoader.h
    
    include/Physics/PhysicsWorld.h
    include/Physics/Collider.h
    include/Physics/Raycast.h
    include/Physics/Trigger.h
    
    include/Scripting/LuaVM.h
    include/Scripting/LuaBindings.h
    include/Scripting/ScriptManager.h
    
    include/Entities/Entity.h
    include/Entities/EntityManager.h
    include/Entities/Components.h
    
    include/AI/AIController.h
    include/AI/Pathfinding.h
    
    include/Weapons/Weapon.h
    include/Weapons/Projectile.h
    
    include/UI/UIManager.h
    include/UI/HUD.h
    
    include/Audio/AudioManager.h
    
    include/Editor/Editor.h
    include/Editor/Viewport.h
    include/Editor/MapSerializer.h
    
    include/Assets/AssetManager.h
)

# Создаём исполняемый файл
add_executable(ResolutionEngine
    src/main.cpp
    ${ALL_SOURCES}
    ${ALL_HEADERS}
)

# Линкуем библиотеки
target_link_libraries(ResolutionEngine
    ${OPENGL_LIBRARIES}
)

# Для Windows добавляем дополнительные библиотеки
if(WIN32)
    target_link_libraries(ResolutionEngine
        winmm
        ws2_32
    )
endif()

# Для Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(ResolutionEngine
        dl
        pthread
    )
endif()

# Копируем данные в папку сборки
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Установка
install(TARGETS ResolutionEngine DESTINATION bin)
install(DIRECTORY data DESTINATION share/resolution-engine)

# Генерация документации (если есть Doxygen)
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

message(STATUS "Resolution Engine configuration complete")
message(STATUS "Build editor: ${BUILD_EDITOR}")
message(STATUS "Build tests: ${BUILD_TESTS}")
