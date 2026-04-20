cmake_minimum_required(VERSION 4.1)
project(arena)

set(CMAKE_CXX_STANDARD 17)

# --- Core library (game logic) ---
add_library(arena_lib
        include/character/Character.h
        model/character/Character.cpp
        include/ConsoleRenderer.h
        model/ConsoleRenderer.cpp
        include/character/Warrior.h
        include/character/Mage.h
        include/character/Archer.h
        include/types/EAction.h
        include/Arena.h
        include/Game.h
        model/character/Warrior.cpp
        model/character/Archer.cpp
        model/character/Mage.cpp
        model/Game.cpp
        model/Arena.cpp
        include/controller/Controller.h
        model/controller/HumanController.cpp
        include/controller/HumanController.h
        model/controller/HardAIController.cpp
        include/controller/HardAIController.h
        model/controller/AIController.cpp
        include/controller/AIController.h
        model/controller/EasyAIController.cpp
        include/controller/EasyAIController.h
        model/SaveManager.cpp
        include/SaveManager.h
        include/types/GameState.h
        include/types/ECharacterType.h
        include/types/EGameCommand.h
        include/types/Color.h
)

target_include_directories(arena_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Application ---
add_executable(arena main.cpp)
target_link_libraries(arena PRIVATE arena_lib)

# --- Tests ---
include(FetchContent)
enable_testing()

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(arena_tests
        tests/catch2_config.cpp
        tests/character/test_character.cpp
        tests/character/test_archer.cpp
        tests/character/test_mage.cpp
        tests/character/test_warrior.cpp
        tests/arena/test_arena.cpp
        tests/game/test_game.cpp
        tests/renderer/test_console_renderer.cpp
        tests/controller/test_human_controller.cpp
        tests/controller/test_ai_controller.cpp
        tests/controller/test_easy_ai_controller.cpp
        tests/controller/test_hard_ai_controller.cpp
        tests/save_manager/test_save_manager.cpp
)
target_link_libraries(arena_tests PRIVATE arena_lib Catch2::Catch2)
target_include_directories(arena_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/character
)

include(CTest)
include(Catch)
catch_discover_tests(arena_tests)

