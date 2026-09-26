#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "enemy.h"

static LootEntry skelly_loot_entries[] = {
    {
        .definition = NULL,
        .weight = 70,
        .min_quantity = 0,
        .max_quantity = 0
    },

    {
        .definition = NULL,
        .weight = 10,
        .min_quantity = 1,
        .max_quantity = 1
    },

    {
        .definition = NULL,
        .weight = 20,
        .min_quantity = 1,
        .max_quantity = 2
    },

    {
        .definition = NULL,
        .weight = 4,
        .min_quantity = 1,
        .max_quantity = 1
    },

    {
        .definition = NULL,
        .weight = 1,
        .min_quantity = 1,
        .max_quantity = 1
    }
};

static LootEntry goblin_loot_entries[] = {
    {
        .definition = NULL,
        .weight = 70,
        .min_quantity = 0,
        .max_quantity = 0
    },

    {
        .definition = NULL,
        .weight = 10,
        .min_quantity = 1,
        .max_quantity = 1
    },

    {
        .definition = NULL,
        .weight = 20,
        .min_quantity = 1,
        .max_quantity = 2
    },

    {
        .definition = NULL,
        .weight = 4,
        .min_quantity = 1,
        .max_quantity = 1
    },

    {
        .definition = NULL,
        .weight = 1,
        .min_quantity = 1,
        .max_quantity = 1
    }
};

static EnemyDefinition enemy_definitions[] = {
    {
        .name = "Skelly The Skeleton",

        .max_hp = 7,
        .damage = 2,
        .exp_reward = 20,

        .aggression = 50,

        .loot_table = {
            .entries = skelly_loot_entries,
            .entry_count =
                sizeof(skelly_loot_entries) /
                sizeof(skelly_loot_entries[0]),
            .rolls = 2
        }
    },

    {
        .name = "Goblin",

        .max_hp = 7,
        .damage = 2,
        .exp_reward = 20,

        .aggression = 70,

        .loot_table = {
            .entries = goblin_loot_entries,
            .entry_count =
                sizeof(goblin_loot_entries) /
                sizeof(goblin_loot_entries[0]),
            .rolls = 2
        }
    }
};

static const int enemy_definition_count =
    sizeof(enemy_definitions) /
    sizeof(enemy_definitions[0]);

static int enemies_initialized = 0;

static void enemy_initialize(void)
{
    if (enemies_initialized)
    {
        return;
    }

    // Skelly loot
    skelly_loot_entries[0].definition = NULL;

    skelly_loot_entries[1].definition =
        item_find("Health Potion");

    skelly_loot_entries[2].definition =
        item_find("Gold Coin");

    skelly_loot_entries[3].definition =
        item_find("Quartz");

    skelly_loot_entries[4].definition =
        item_find("Amethyst");

    // Goblin loot
    goblin_loot_entries[0].definition = NULL;

    goblin_loot_entries[1].definition =
        item_find("Health Potion");

    goblin_loot_entries[2].definition =
        item_find("Gold Coin");

    goblin_loot_entries[3].definition =
        item_find("Quartz");

    goblin_loot_entries[4].definition =
        item_find("Amethyst");

    enemies_initialized = 1;
}

const EnemyDefinition *enemy_find(
    const char *name)
{
    enemy_initialize();

    for (int i = 0;
         i < enemy_definition_count;
         i++)
    {
        if (strcmp(
                enemy_definitions[i].name,
                name) == 0)
        {
            return &enemy_definitions[i];
        }
    }

    return NULL;
}

EnemyAction enemy_choose_action(
    const EnemyDefinition *enemy,
    int enemy_hp,
    int player_hp,
    int player_max_hp)
{
    int quick_weight = 40;
    int heavy_weight = 20;
    int defend_weight = 20;

    int enemy_hp_percent =
        enemy_hp * 100 / enemy->max_hp;

    int player_hp_percent =
        player_hp * 100 / player_max_hp;

    quick_weight +=
        enemy->aggression / 2;

    heavy_weight +=
        enemy->aggression;

    defend_weight +=
        100 - enemy->aggression;

    if (enemy_hp_percent <= 30)
    {
        defend_weight += 40;
    }
    else if (enemy_hp_percent <= 50)
    {
        defend_weight += 20;
    }

    if (player_hp_percent <= 30)
    {
        quick_weight += 30;
        heavy_weight += 30;
    }
    else if (player_hp_percent <= 50)
    {
        quick_weight += 15;
        heavy_weight += 15;
    }

    int total_weight =
        quick_weight +
        heavy_weight +
        defend_weight;

    int random_value =
        rand() % total_weight;

    if (random_value < quick_weight)
    {
        return ENEMY_QUICK_ATTACK;
    }

    random_value -= quick_weight;

    if (random_value < heavy_weight)
    {
        return ENEMY_HEAVY_ATTACK;
    }

    return ENEMY_DEFEND;
}