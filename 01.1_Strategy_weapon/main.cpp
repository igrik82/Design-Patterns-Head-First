#include "Character.h"
#include "Weapon.h"

int main()
{
    King king;
    king.fight();
    king.setWeapon(std::make_unique<AxeBehavior>());
    king.fight();

    Queen queen;
    queen.fight();
    queen.setWeapon(std::make_unique<BowAndArrowBehavior>());
    queen.fight();

    Troll troll;
    troll.fight();
    troll.setWeapon(std::make_unique<KnifeBehavior>());
    troll.fight();

    return 0;
}
