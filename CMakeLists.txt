cmake_minimum_required(VERSION 4.1)
project(arena)

set(CMAKE_CXX_STANDARD 17)

add_executable(arena main.cpp
        include/character/Character.h
        model/character/Character.cpp
        include/character/Warrior.h
        include/character/Mage.h
        include/character/Archer.h
        include/EAction.h
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
        include/GameState.h
        include/ECharacterType.h
)
