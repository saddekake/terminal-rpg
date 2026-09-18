#include <stdio.h>
#include <stdlib.h>

#include "combat.h"

// Like Skelly himself, I'm hardcoding this until v0.4 when I'm implementing enemy defs. Want to pace myself
static const LootEntry skelly_loot_entries[] = {
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

static const LootTable skelly_loot_table = {
    .entries = skelly_loot_entries,
    .entry_count =
        sizeof(skelly_loot_entries) /
        sizeof(skelly_loot_entries[0]),
    .rolls = 2
};

static void combat_generate_loot(
    Combat *combat,
    Player *player)
{
    LootEntry entries[
        sizeof(skelly_loot_entries) /
        sizeof(skelly_loot_entries[0])
    ];

    for (int i = 0;
         i < skelly_loot_table.entry_count;
         i++)
    {
        entries[i] =
            skelly_loot_entries[i];
    }

    entries[0].definition = NULL;

    entries[1].definition =
        item_find("Health Potion");

    entries[2].definition =
        item_find("Gold Coin");

    entries[3].definition =
        item_find("Quartz");

    entries[4].definition =
        item_find("Amethyst");

    LootTable table = {
        .entries = entries,
        .entry_count =
            skelly_loot_table.entry_count,
        .rolls = skelly_loot_table.rolls
    };

    LootResults results;

    loot_generate(
        &table,
        player,
        &results);

    for (int i = 0;
         i < results.result_count;
         i++)
    {
        player_add_item(
            player,
            results.results[i].definition,
            results.results[i].quantity);
    }

    combat->loot_results = results;
    combat->loot_active = 1;
}

void combat_start(Combat *combat, Player *player)
{
    combat->active = 1;
    combat->loot_active = 0;

    combat->enemy_hp = 7;
    combat->enemy_max_hp = 7;

    player->hp = player->max_hp;

    combat->player_defending = 0;

    combat->player_decision = 0;
    combat->enemy_decision = 0;

    combat->player_damage_dealt = 0;
    combat->enemy_damage_dealt = 0;

    combat->loot_results.result_count = 0;
}

void combat_render(
    const Combat *combat,
    const Player *player)
{
    printf("Skelly The Skeleton\n");
    printf("HP: %d/%d\n\n",
           combat->enemy_hp,
           combat->enemy_max_hp);

    printf("You\n");
    printf("HP: %d/%d\n\n",
           player->hp,
           player->max_hp);

    printf("1. Quick Attack\n");
    printf("2. Heavy Attack\n");
    printf("3. Defend\n");
    printf("4. Inventory\n");
    printf("5. Run\n\n");

    if (combat->player_decision == 1)
    {
        printf("You decided: Quick Attack\n");
    }
    else if (combat->player_decision == 2)
    {
        printf("You decided: Heavy Attack\n");
    }
    else if (combat->player_decision == 3)
    {
        printf("You decided: Defend\n");
    }
    else if (combat->player_decision == 4)
    {
        printf("You decided: Inventory\n");
    }
    else if (combat->player_decision == 5)
    {
        printf("You decided: Run\n");
    }
    else
    {
        printf("You decided:\n");
    }

    if (combat->enemy_decision == 1)
    {
        printf("Enemy decided: Default Attack\n");
    }
    else
    {
        printf("Enemy decided:\n");
    }

    if (combat->player_decision == 1 ||
        combat->player_decision == 2)
    {
        printf("You dealt %d damage.\n",
               combat->player_damage_dealt);
    }

    if (combat->enemy_decision == 1)
    {
        printf("Skelly dealt %d damage.\n",
               combat->enemy_damage_dealt);
    }
}

void combat_handle_input(
    Combat *combat,
    Player *player,
    char input)
{
    // Ignore invalid input
    if (input != '1' &&
        input != '2' &&
        input != '3' &&
        input != '4' &&
        input != '5')
    {
        return;
    }

    // Reset turn results
    combat->player_damage_dealt = 0;
    combat->enemy_damage_dealt = 0;

    // Quick Attack
    if (input == '1')
    {
        combat->player_decision = 1;

        // Player attacks
        combat->player_damage_dealt =
            player->weapons[player->current_weapon]
                .definition->damage;

        combat->enemy_hp -=
            combat->player_damage_dealt;

        // Check if enemy died
        if (combat->enemy_hp <= 0)
        {
            combat->enemy_hp = 0;

            player_add_exp(player, 20);

            combat_generate_loot(
                combat,
                player);

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }

        // Enemy attacks
        combat->enemy_decision = 1;

        int damage = 2;

        damage -=
            player->armor[player->current_armor]
                .definition->damage_negation;

        if (damage < 0)
        {
            damage = 0;
        }

        if (combat->player_defending)
        {
            damage /= 2;
            combat->player_defending = 0;
        }

        combat->enemy_damage_dealt = damage;

        player->hp -= damage;

        if (player->hp <= 0)
        {
            player->hp = 0;

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }
    }

    // Heavy Attack
    else if (input == '2')
    {
        combat->player_decision = 2;

        // 70% chance to hit
        int hit_chance = rand() % 100;

        if (hit_chance < 70)
        {
            combat->player_damage_dealt =
                player->weapons[player->current_weapon]
                    .definition->damage * 2;

            combat->enemy_hp -=
                combat->player_damage_dealt;

            if (combat->enemy_hp <= 0)
            {
                combat->enemy_hp = 0;

                player_add_exp(player, 20);

                combat_generate_loot(
                    combat,
                    player);

                combat->active = 0;

                combat->player_decision = 0;
                combat->enemy_decision = 0;

                return;
            }
        }

        // Enemy attacks
        combat->enemy_decision = 1;

        int damage = 2;

        damage -=
            player->armor[player->current_armor]
                .definition->damage_negation;

        if (damage < 0)
        {
            damage = 0;
        }

        if (combat->player_defending)
        {
            damage /= 2;
            combat->player_defending = 0;
        }

        combat->enemy_damage_dealt = damage;

        player->hp -= damage;

        if (player->hp <= 0)
        {
            player->hp = 0;

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }
    }

    // Defend
    else if (input == '3')
    {
        combat->player_decision = 3;
        combat->player_defending = 1;

        // Enemy attacks
        combat->enemy_decision = 1;

        int damage = 2;

        damage -=
            player->armor[player->current_armor]
                .definition->damage_negation;

        if (damage < 0)
        {
            damage = 0;
        }

        if (combat->player_defending)
        {
            damage /= 2;
            combat->player_defending = 0;
        }

        combat->enemy_damage_dealt = damage;

        player->hp -= damage;

        if (player->hp <= 0)
        {
            player->hp = 0;

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }
    }

    // Inventory
    else if (input == '4')
    {
        combat->player_decision = 4;

        return;
    }

    // Run
    else if (input == '5')
    {
        combat->active = 0;

        combat->player_decision = 0;
        combat->enemy_decision = 0;

        combat->player_damage_dealt = 0;
        combat->enemy_damage_dealt = 0;

        return;
    }
}