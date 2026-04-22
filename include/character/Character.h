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
    int teamId;
    int hp;
    int maxHp;
    int attack;
    int defense;
    double critChance;
    bool defending;
    double dodgeChance;
    int specialCooldownTurns = 0;
    int specialCooldownRemaining = 0;

protected:
    void setSpecialCooldownTurns(int turns);

public:
    Character(std::string name, int teamId, int hp, int attack, int defense, double critChance, double dodgeChance);
    virtual ~Character() = default;
    virtual int specialAbility(const Character& target) = 0;

    std::string getName() const;
    int getTeamId() const;
    void setTeamId(int id);
    int getHp() const;
    int getMaxHp() const;
    int getAttack() const;
    double getDodgeChance() const;
    int getDefense() const;
    double getCritChance() const;
    bool isDefending() const;
    bool isAlive() const;
    void setDefend(bool defending);
    void defend();
    void takeDamage(int damage);
    void setHealth(int h);
    void setAttack(int a);
    bool canUseSpecial() const;
    void tickSpecialCooldown();
    void startSpecialCooldown();
    int getSpecialCooldownRemaining() const;
    virtual ECharacterType getCharacterType() const = 0;
};

#endif //ARENA_CHARACTER_H