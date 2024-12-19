#include <iostream>
#include <memory>

class QuackBehavior {
public:
    virtual void quack() const = 0;
    // In C++ a base class with virtual function must have a virtual destructor
    virtual ~QuackBehavior() = default;
};

class Quack : public QuackBehavior {
public:
    void quack() const override { std::cout << "Quack" << std::endl; }
};

class Duck {
public:
    // In c++ prefer unique_ptr by default, shared_ptr for a multiple references
    std::unique_ptr<QuackBehavior> quackBehavior;

public:
    Duck(std::unique_ptr<QuackBehavior> quackBehavior)
        : quackBehavior(std::move(quackBehavior))
    {
    }

    void performQuack() { quackBehavior->quack(); }
};

class MallardDuck : public Duck {
public:
    MallardDuck()
        : Duck(std::make_unique<Quack>()) { };

    void display() { std::cout << "I'm a real Mallard duck" << std::endl; }
};

int main()
{
    MallardDuck mallarDuck;
    mallarDuck.performQuack();
    mallarDuck.display();

    return 0;
}
