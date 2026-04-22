//
// Created by kamil on 11.03.2026.
//

#include "../../include/character/Character.h"
#include "../../include/templates/MathClamp.h"

#include <utility>

Character::Character(std::string name, int teamId, int hp, int attack, int defense, double critChance, double dodgeChance)
    : name(std::move(name)),
    teamId(teamId),
    hp(hp),
    maxHp(hp),
    attack(attack),
    defense(defense),
    critChance(critChance),
    defending(false),
    dodgeChance(dodgeChance)
    {}

void Character::setDefend(bool isDefending) {
    defending = isDefending;
}

void Character::defend() {
    defending = true;
}

void Character::takeDamage(int damage) {
    hp -= damage;
    hp = clampMin(hp, 0);
}

void Character::setHealth(int h) {
    hp = h;
}

void Character::setAttack(int a) {
    attack = a;
}

bool Character::canUseSpecial() const {
    return specialCooldownRemaining == 0;
}

void Character::tickSpecialCooldown() {
    if (specialCooldownRemaining > 0) {
        --specialCooldownRemaining;
    }
}

void Character::startSpecialCooldown() {
    specialCooldownRemaining = specialCooldownTurns;
}

int Character::getSpecialCooldownRemaining() const {
    return specialCooldownRemaining;
}

void Character::setSpecialCooldownTurns(int turns) {
    specialCooldownTurns = clampMin(turns, 0);
}

int Character::getAttack() const {
    return attack;
}

bool Character::isAlive() const {
    return hp > 0;
}

int Character::getHp() const {
    return hp;
}

int Character::getMaxHp() const {
    return maxHp;
}

std::string Character::getName() const {
    return name;
}

int Character::getTeamId() const {
    return teamId;
}

void Character::setTeamId(int id) {
    teamId = id;
}

double Character::getDodgeChance() const {
    return dodgeChance;
}

double Character::getCritChance() const {
    return critChance;
}

int Character::getDefense() const {
    return defense;
}

bool Character::isDefending() const {
    return defending;
}
