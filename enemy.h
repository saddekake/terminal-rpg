#ifndef ENEMY_H
#define ENEMY_H

#include "loot.h"

typedef enum
{
    ENEMY_QUICK_ATTACK,
    ENEMY_HEAVY_ATTACK,
    ENEMY_DEFEND

} EnemyAction;

typedef struct
{
    const char *name;

    int max_hp;
    int damage;
    int exp_reward;

    int aggression;

    LootTable loot_table;

} EnemyDefinition;

const EnemyDefinition *enemy_find(
    const char *name
);

EnemyAction enemy_choose_action(
    const EnemyDefinition *enemy,
    int enemy_hp,
    int player_hp,
    int player_max_hp
);

#endif