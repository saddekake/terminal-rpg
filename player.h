#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    int x;
    int y;

    int hp;
    int max_hp;

    int level;
    int exp;
    int exp_required;

} Player;

void player_init(Player *player);

void player_move(Player *player, int x, int y);

void player_add_exp(Player *player, int amount);

#endif