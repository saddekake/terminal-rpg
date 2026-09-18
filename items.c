#include <stddef.h>
#include <string.h>

#include "items.h"

static const ItemDefinition item_definitions[] = { // Item definitions, new items go here if you want to add them (wink wink modders)
    {
        .name = "Rusty Copper Sword",
        .category = ITEM_WEAPON,
        .damage = 2
    },

    {
        .name = "Rusty Copper Armor",
        .category = ITEM_ARMOR,
        .damage_negation = 1
    },

    {
        .name = "Health Potion",
        .category = ITEM_CONSUMABLE,
        .healing = 5
    },

    {
        .name = "Placeholder Potion",
        .category = ITEM_ITEM
    },
    {
        .name = "Fox Plushie",
        .category = ITEM_ITEM
    },
    {
        .name = "Gold Coin",
        .category = ITEM_ITEM
    },
    {
        .name = "Quartz",
        .category = ITEM_ITEM
    },
    {
        .name = "Amethyst",
        .category = ITEM_ITEM
    },
    {
        .name = "Emerald",
        .category = ITEM_ITEM
    },
    {
        .name = "Ruby",
        .category = ITEM_ITEM
    },
    {
        .name = "Sapphire",
        .category = ITEM_ITEM
    },
    {
        .name = "Diamond",
        .category = ITEM_ITEM
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