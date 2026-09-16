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