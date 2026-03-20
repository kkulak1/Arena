//
// Created by kamil on 11.03.2026.
//

#include "../../include/character/Character.h"

#include <iostream>
#include <string>

Character::Character(std::string name, int hp, int attack, int defense, double critChance, double dodgeChance)
    : name(name),
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
        std::cout << target.getName() << " dodged the attack!\n";
        return;
    }

    int damage = getAttack() - target.getDefense();

    if (damage < 0) {
        damage = 0;
    }

    if (double critRoll = (double) rand() / RAND_MAX; critRoll < critChance) {
        damage *= 2;
        std::cout << "Critical hit";
    }

    target.takeDamage(damage);

    std::cout << name << " deals " << damage << " damage to " << target.getName() << std::endl;
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
