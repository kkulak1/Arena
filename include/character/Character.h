//
// Created by kamil on 11.03.2026.
//

#ifndef ARENA_CHARACTER_H
#define ARENA_CHARACTER_H
#include <string>

#include "../types/ECharacterType.h"

class Character {
private:
    std::string name;
    int hp;
    int maxHp;
    int attack;
    int defense;
    double critChance;
    bool defending;
    double dodgeChance;

public:
    Character(std::string name, int hp, int attack, int defense, double critChance, double dodgeChance);
    virtual ~Character() = default;
    virtual void specialAbility(Character& target) = 0;

    std::string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    double getDodgeChance() const;
    int getDefense() const;
    double getCritChance() const;
    bool isDefending() const;
    bool isAlive() const;
    void defend();
    void takeDamage(int damage);
    void attackTarget(Character& target);


    void setHealth(int h);
    void setAttack(int a);
    virtual ECharacterType getCharacterType() const = 0;
};

#endif //ARENA_CHARACTER_H