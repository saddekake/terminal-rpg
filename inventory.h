#ifndef INVENTORY_H
#define INVENTORY_H

#include "player.h"

#define INVENTORY_VISIBLE_ITEMS 5

typedef enum
{
    INVENTORY_NORMAL,
    INVENTORY_COMBAT

} InventoryMode;

typedef struct
{
    int active;

    InventoryMode mode;

    int selected_category;
    int selected_item;
    int item_scroll;

} Inventory;

void inventory_toggle(Inventory *inventory);

void inventory_open(
    Inventory *inventory,
    InventoryMode mode
);

void inventory_close(Inventory *inventory);

void inventory_render(
    const Inventory *inventory,
    const Player *player
);

void inventory_handle_input(
    Inventory *inventory,
    Player *player,
    char input
);

#endif