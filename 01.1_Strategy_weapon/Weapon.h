/*
 * Абстрактный класс-интерфейс weapon
 */
#pragma once

#include <iostream>

// Абстрактный класс с обязательной реализацией метода в наследующих классах
class WeaponType {
public:
    virtual void useWeapon() const = 0;
    virtual ~WeaponType() = default;
};

class SwordBehavior : public WeaponType {
    void useWeapon() const override
    {
        std::cout << "I'm fighting with sword!" << std::endl;
    }
};

class KnifeBehavior : public WeaponType {
    void useWeapon() const override
    {
        std::cout << "I'm fighting with sword!" << std::endl;
    }
};

class BowAndArrowBehavior : public WeaponType {
    void useWeapon() const override
    {
        std::cout << "I'm shooting with bow!" << std::endl;
    }
};

class AxeBehavior : public WeaponType {
    void useWeapon() const override
    {
        std::cout << "I'm fighting with axe!" << std::endl;
    }
};
