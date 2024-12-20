/*
 * Базовый класс утки, который наследуется каждым конкретным видом уток,
 * имеет только базовые возможности(крякать, летать и т.д.).
 * Конкретная реализация кряканья и полета реализуется в отдельных классах,
 * для возможности выбора конкретного типа.
 */
#pragma once

#include "FlyBehavior.h"
#include "QuackBehavior.h"
#include <memory>

class Duck {
public:
    // In c++ prefer unique_ptr by default, shared_ptr for a multiple references
    std::unique_ptr<QuackBehavior> quackBehavior;
    std::unique_ptr<FlyBehavior> flyBehavior;

    Duck(std::unique_ptr<QuackBehavior> quackBehavior,
        std::unique_ptr<FlyBehavior> flyBehavior)
        : quackBehavior(std::move(quackBehavior))
        , flyBehavior(std::move(flyBehavior))
    {
    }

    void performQuack() { quackBehavior->quack(); }
    void performFly() { flyBehavior->fly(); }

    void setFlyBehavior(std::unique_ptr<FlyBehavior> flyBehavior)
    {
        this->flyBehavior = std::move(flyBehavior);
    }

    void setQuackBehavior(std::unique_ptr<QuackBehavior> quackBehavior)
    {
        this->quackBehavior = std::move(quackBehavior);
    }
};

class MallardDuck : public Duck {
public:
    // Конструктор который принимает вызов конструктора базового класса в который
    // передается два уникальных указателя на FlyBehavior и QuackBehavior с
    // вызовом.
    MallardDuck()
        : Duck(std::make_unique<Quack>(), std::make_unique<FlyWithWings>())
    {
        std::cout << "\n-------- MallardDuck --------" << std::endl;
    };

    void display() { std::cout << "I'm a real Mallard duck" << std::endl; }
};

class ModelDuck : public Duck {
public:
    ModelDuck()
        : Duck(std::make_unique<Silence>(), std::make_unique<FlyNoWay>())
    {
        std::cout << "\n-------- ModelDuck --------" << std::endl;
    }

    void display() { std::cout << "I'm a model duck" << std::endl; }
};
