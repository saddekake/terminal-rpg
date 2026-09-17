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
    player->weapons[0].name = "Rusty Copper Sword";
    player->weapons[0].damage = 2;
    player->weapon_count = 1;
    player->current_weapon = 0;

    // Starting armor
    player->armor[0].name = "Rusty Copper Armor";
    player->armor[0].damage_negation = 1;
    player->armor_count = 1;
    player->current_armor = 0;

    // Starting items
    player->items[0].name = "Health Potion";
    player->items[0].type = ITEM_HEALTH_POTION;
    player->items[0].healing = 5;
    player->items[0].quantity = 1;

    player->items[1].name = "Placeholder Potion";
    player->items[1].type = ITEM_PLACEHOLDER_POTION;
    player->items[1].healing = 0;
    player->items[1].quantity = 1;

    player->item_count = 2;
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
            (int)(player->exp_required * 1.10);
    }
}