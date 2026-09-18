#ifndef PLAYER_H
#define PLAYER_H

#include "items.h"

#define MAX_WEAPONS 10
#define MAX_ARMOR 10
#define MAX_ITEMS 20

typedef struct
{
    const ItemDefinition *definition;
    int quantity;

} InventoryItem;

typedef struct
{
    int x;
    int y;

    int hp;
    int max_hp;

    int level;
    int exp;
    int exp_required;

    InventoryItem weapons[MAX_WEAPONS];
    int weapon_count;
    int current_weapon;

    InventoryItem armor[MAX_ARMOR];
    int armor_count;
    int current_armor;

    InventoryItem items[MAX_ITEMS];
    int item_count;

} Player;

void player_init(Player *player);
void player_move(Player *player, int x, int y);
void player_add_exp(Player *player, int amount);

void player_add_item(
    Player *player,
    const ItemDefinition *definition,
    int quantity
);

#endif