#ifndef ITEMS_H
#define ITEMS_H

typedef enum
{
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_CONSUMABLE,
    ITEM_ITEM

} ItemCategory;

typedef struct
{
    const char *name;

    ItemCategory category;

    int damage;
    int damage_negation;
    int healing;

    int unique;

} ItemDefinition;

const ItemDefinition *item_find(
    const char *name
);

#endif