cmake_minimum_required(VERSION 4.1)
project(arena)

set(CMAKE_CXX_STANDARD 17)

add_executable(arena main.cpp
        include/character/Character.h
        model/character/Character.cpp
        include/character/Warrior.h
        include/character/Mage.h
        include/character/Archer.h
        include/Action.h
        include/Arena.h
        include/Game.h
        include/controller/AIController.h
        model/character/Warrior.cpp
        model/character/Archer.cpp
        model/character/Mage.cpp
        model/Game.cpp
        model/Arena.cpp
        model/controller/AIController.cpp
        include/controller/Controller.h
        model/controller/HumanController.cpp
        include/controller/HumanController.h
)
