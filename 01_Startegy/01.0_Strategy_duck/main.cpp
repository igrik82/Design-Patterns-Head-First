#include "Duck.h"

int main()
{
    MallardDuck mallarDuck;
    mallarDuck.performQuack();
    mallarDuck.performFly();
    mallarDuck.display();
    mallarDuck.setFlyBehavior(std::make_unique<FlyNoWay>());
    mallarDuck.performFly();

    ModelDuck modelDuck;
    modelDuck.performQuack();
    modelDuck.performFly();
    modelDuck.display();
    modelDuck.setFlyBehavior(std::make_unique<FlyRocketPowered>());
    modelDuck.performFly();

    return 0;
}
