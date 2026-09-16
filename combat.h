#ifndef COMBAT_H
#define COMBAT_H

#include "player.h"

typedef struct
{
    int active;

    int enemy_hp;
    int enemy_max_hp;

    int player_defending;

    int player_decision;
    int enemy_decision;

    int player_damage_dealt;
    int enemy_damage_dealt;

} Combat;

void combat_start(Combat *combat, Player *player);

void combat_render(const Combat *combat, const Player *player);

void combat_handle_input(
    Combat *combat,
    Player *player,
    char input
);

#endif