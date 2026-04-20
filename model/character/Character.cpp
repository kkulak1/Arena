//
// Created by kamil on 11.03.2026.
//

#include "../../include/character/Character.h"
#include "../../include/ConsoleRenderer.h"
#include "../../include/types/Color.h"

#include <string>
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

void Character::defend() {
    defending = true;
}

void Character::takeDamage(int damage) {
    if (defending) {
        damage /= 2;
        defending = false;
    }

    hp -= damage;

    if (hp < 0) {
        hp = 0;
    }
}

void Character::attackTarget(Character &target) {
    double dodgeRoll = (double) rand() / RAND_MAX;
    if (dodgeRoll < target.getDodgeChance()) {
        ConsoleRenderer::printMessage(target.getName() + " dodged the attack!", Color::Default, &target);
        return;
    }

    int damage = getAttack() - target.getDefense();

    if (damage < 0) {
        damage = 0;
    }

    if (double critRoll = (double) rand() / RAND_MAX; critRoll < critChance) {
        damage *= 2;
        ConsoleRenderer::printMessage("Critical hit", Color::Default);
    }

    target.takeDamage(damage);

    ConsoleRenderer::printMessage(name + " deals " + std::to_string(damage) + " damage to " + target.getName(), Color::Default, this);
}


void Character::setHealth(int h) {
    hp = h;
}

void Character::setAttack(int a) {
    attack = a;
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
