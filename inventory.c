#include <stdio.h>

#include "inventory.h"

void inventory_toggle(Inventory *inventory)
{
    inventory->active = !inventory->active;
}

void inventory_render(const Player *player)
{
    printf("INVENTORY\n\n");

    printf("Level: %d\n", player->level);
    printf("EXP: %d/%d\n",
           player->exp,
           player->exp_required);
}