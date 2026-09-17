#ifndef PLAYER_H
#define PLAYER_H

#define MAX_WEAPONS 10
#define MAX_ARMOR 10
#define MAX_ITEMS 20

typedef struct
{
    const char *name;
    int damage;

} Weapon;

typedef struct
{
    const char *name;
    int damage_negation;

} Armor;

typedef enum
{
    ITEM_HEALTH_POTION,
    ITEM_PLACEHOLDER_POTION

} ItemType;

typedef struct
{
    const char *name;
    ItemType type;
    int healing;
    int quantity;

} Item;

typedef struct
{
    int x;
    int y;

    int hp;
    int max_hp;

    int level;
    int exp;
    int exp_required;

    Weapon weapons[MAX_WEAPONS];
    int weapon_count;
    int current_weapon;

    Armor armor[MAX_ARMOR];
    int armor_count;
    int current_armor;

    Item items[MAX_ITEMS];
    int item_count;

} Player;

void player_init(Player *player);
void player_move(Player *player, int x, int y);
void player_add_exp(Player *player, int amount);

#endif