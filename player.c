#include <stddef.h>

#include "player.h"

void player_init(Player *player)
{
    player->x = 9;
    player->y = 2;

    player->hp = 10;
    player->max_hp = 10;

    player->level = 1;
    player->exp = 0;
    player->exp_required = 100;

    // Starting weapon
    player->weapons[0].definition =
        item_find("Damaged Copper Sword");

    player->weapons[0].quantity = 1;

    player->weapon_count = 1; // Update this count if you add something new to the player starting inventory
    player->current_weapon = 0;

    // Starting armor
    player->armor[0].definition =
        item_find("Damaged Copper Armor");

    player->armor[0].quantity = 1;

    player->armor_count = 1; // Update this count if you add something new to the player starting inventory
    player->current_armor = 0;

    // Starting items
    player->items[0].definition =
        item_find("Health Potion");

    player->items[0].quantity = 1;

    player->items[1].definition =
        item_find("Fox Plushie");

    player->items[1].quantity = 1;

    player->items[2].definition =
        item_find("Gold Coin");

    player->items[2].quantity = 2;

    player->items[3].definition =
        item_find("Quartz");

    player->items[3].quantity = 1;

    player->items[4].definition =
        item_find("Amethyst");

    player->items[4].quantity = 1;

    player->items[5].definition =
        item_find("Emerald");

    player->items[5].quantity = 1;

    player->item_count = 6; // Update this count if you add something new to the player starting inventory
}

void player_move(Player *player, int x, int y)
{
    player->x = x;
    player->y = y;
}

void player_add_exp(Player *player, int amount)
{
    player->exp += amount;

    while (player->exp >= player->exp_required)
    {
        player->exp -= player->exp_required;

        player->level++;

        player->exp_required =
            (int)(player->exp_required * 1.20);
    }
}

void player_add_item(
    Player *player,
    const ItemDefinition *definition,
    int quantity)
{
    if (definition == NULL ||
        quantity <= 0)
    {
        return;
    }

    // Weapons
    if (definition->category == ITEM_WEAPON)
    {
        for (int i = 0;
             i < player->weapon_count;
             i++)
        {
            if (player->weapons[i].definition ==
                definition)
            {
                player->weapons[i].quantity +=
                    quantity;

                return;
            }
        }

        if (player->weapon_count >= MAX_WEAPONS)
        {
            return;
        }

        player->weapons[
            player->weapon_count
        ].definition = definition;

        player->weapons[
            player->weapon_count
        ].quantity = quantity;

        player->weapon_count++;

        return;
    }

    // Armor
    if (definition->category == ITEM_ARMOR)
    {
        for (int i = 0;
             i < player->armor_count;
             i++)
        {
            if (player->armor[i].definition ==
                definition)
            {
                player->armor[i].quantity +=
                    quantity;

                return;
            }
        }

        if (player->armor_count >= MAX_ARMOR)
        {
            return;
        }

        player->armor[
            player->armor_count
        ].definition = definition;

        player->armor[
            player->armor_count
        ].quantity = quantity;

        player->armor_count++;

        return;
    }

    // Items and consumables
    for (int i = 0;
         i < player->item_count;
         i++)
    {
        if (player->items[i].definition ==
            definition)
        {
            player->items[i].quantity +=
                quantity;

            return;
        }
    }

    if (player->item_count >= MAX_ITEMS)
    {
        return;
    }

    player->items[
        player->item_count
    ].definition = definition;

    player->items[
        player->item_count
    ].quantity = quantity;

    player->item_count++;
}