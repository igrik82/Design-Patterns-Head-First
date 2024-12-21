
/*
 * Абстрактный класс QuackBehavior позволяет упорядочить реализацию
 * конкретного типа кряканья.
 */
#pragma once

#include <iostream>

// Абстрактный класс с обязательной реализацией метода в наследующих классах
class QuackBehavior {
public:
    // Обязательный метод для реализации =0
    virtual void quack() const = 0;
    // В виртуальном базовом классе с виртуальным методом
    // обязательно должен быть виртуальный деструктор
    virtual ~QuackBehavior() = default;
};

// Класс в котором утки крякают
class Quack : public QuackBehavior {
public:
    void quack() const override { std::cout << "Quack" << std::endl; }
};

// Класс в котором утки пищат
class Squeak : public QuackBehavior {
public:
    void quack() const override { std::cout << "Squeak" << std::endl; }
};

// Класс в котором утки не издают звуков
class Silence : public QuackBehavior {
public:
    void quack() const override { std::cout << "<< Silence >>" << std::endl; }
};
