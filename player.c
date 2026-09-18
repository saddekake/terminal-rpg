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
        item_find("Rusty Copper Sword");

    player->weapons[0].quantity = 1;

    player->weapon_count = 1;
    player->current_weapon = 0;

    // Starting armor
    player->armor[0].definition =
        item_find("Rusty Copper Armor");

    player->armor[0].quantity = 1;

    player->armor_count = 1;
    player->current_armor = 0;

    // Starting items
    player->items[0].definition =
        item_find("Health Potion");

    player->items[0].quantity = 1;

    player->items[1].definition =
        item_find("Placeholder Potion");

    player->items[1].quantity = 1;

        player->items[2].definition =
        item_find("Fox Plushie");

    player->items[2].quantity = 1;

    player->items[3].definition =
        item_find("Gold Coin");

    player->items[3].quantity = 2;

    player->items[4].definition =
        item_find("Quartz");

    player->items[4].quantity = 1;

    player->items[5].definition =
        item_find("Amethyst");

    player->items[5].quantity = 1;

    player->item_count = 6;
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