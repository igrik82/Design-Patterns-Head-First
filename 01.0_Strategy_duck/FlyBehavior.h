/*
 * Абстрактный класс FlyBehavior позволяет упорядочить реализацию
 * конкретного типа полета.
 */
#pragma once

#include <iostream>

// Абстрактный класс с обязательной реализацией метода в наследующих классах
class FlyBehavior {
public:
    // Обязательный метод для реализации =0
    virtual void fly() const = 0;
    // В виртуальном классе обязательно должен быть виртуальный деструктор
    virtual ~FlyBehavior() = default;
};

// Класс в котором утки летают
class FlyWithWings : public FlyBehavior {
public:
    void fly() const override { std::cout << "I'm flying!" << std::endl; }
};

// Класс в котором утки не летают
class FlyNoWay : public FlyBehavior {
    void fly() const override { std::cout << "I can't fly" << std::endl; }
};

// Класс полета с ракетой
class FlyRocketPowered : public FlyBehavior {
public:
    void fly() const override
    {
        std::cout << "I'm flying with a rocket!" << std::endl;
    }
};
