/*
 * Базовый класс персонажа
 */
#pragma once

#include "Weapon.h"
#include <memory>

class Character {
public:
    std::unique_ptr<WeaponType> weaponType;

    Character(std::unique_ptr<WeaponType> weaponType)
        : weaponType(std::move(weaponType))
    {
    }

    void fight() { weaponType->useWeapon(); }

    void setWeapon(std::unique_ptr<WeaponType> weaponType)
    {
        std::cout << "Changing weapon" << std::endl;
        this->weaponType = std::move(weaponType);
    }
};

class King : public Character {
public:
    King()
        : Character(std::make_unique<SwordBehavior>())
    {
        std::cout << "\n--------- King ---------" << std::endl;
    }

    void display() { std::cout << "I'm King" << std::endl; }
};

class Queen : public Character {
public:
    Queen()
        : Character(std::make_unique<KnifeBehavior>())
    {
        std::cout << "\n--------- Queen ---------" << std::endl;
    }
    void display() { std::cout << "I'm Queen" << std::endl; }
};

class Troll : public Character {
public:
    Troll()
        : Character(std::make_unique<AxeBehavior>())
    {
        std::cout << "\n--------- Troll ---------" << std::endl;
    }
    void display() { std::cout << "I'm Troll" << std::endl; }
};

class Knight : public Character {
public:
    Knight()
        : Character(std::make_unique<BowAndArrowBehavior>())
    {
        std::cout << "\n-------- Knight --------" << std::endl;
    }
    void display() { std::cout << "I'm Knight" << std::endl; }
};
