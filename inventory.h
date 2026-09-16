#ifndef INVENTORY_H
#define INVENTORY_H

#include "player.h"

typedef struct
{
    int active;

} Inventory;

void inventory_toggle(Inventory *inventory);

void inventory_render(const Player *player);

#endif
