#include <stddef.h>
#include <string.h>

#include "items.h"
/* 
    Item definitions, new items go here if you want to add them (wink wink modders)
    Remember to add them to player_init() in player.c if you want them to appear in the starting inventory
*/
static const ItemDefinition item_definitions[] = {
    // Weapons
    {
        .name = "Damaged Copper Sword",
        .category = ITEM_WEAPON,
        .damage = 2,
        .unique = 1
    },
    {
        .name = "Bronze Sword",
        .category = ITEM_WEAPON,
        .damage = 4,
        .unique = 1
    },

    // Armor
    {
        .name = "Damaged Copper Armor",
        .category = ITEM_ARMOR,
        .damage_negation = 1,
        .unique = 1
    },
    {
        .name = "Bronze Armor",
        .category = ITEM_ARMOR,
        .damage_negation = 2,
        .unique = 1
    },

    // Consumables
    {
        .name = "Health Potion",
        .category = ITEM_CONSUMABLE,
        .healing = 5,
        .unique = 0
    },

    // Items
    {
        .name = "Fox Plushie",
        .category = ITEM_ITEM,
        .unique = 1
    },
    {
        .name = "Gold Coin",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Quartz",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Amethyst",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Emerald",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Ruby",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Sapphire",
        .category = ITEM_ITEM,
        .unique = 0
    },
    {
        .name = "Diamond",
        .category = ITEM_ITEM,
        .unique = 0
    },
};

static const int item_definition_count =
    sizeof(item_definitions) /
    sizeof(item_definitions[0]);

const ItemDefinition *item_find(
    const char *name)
{
    for (int i = 0;
         i < item_definition_count;
         i++)
    {
        if (strcmp(
                item_definitions[i].name,
                name) == 0)
        {
            return &item_definitions[i];
        }
    }

    return NULL;
}