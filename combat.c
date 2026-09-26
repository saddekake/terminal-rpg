#include <stdio.h>
#include <stdlib.h>

#include "combat.h"

static void combat_generate_loot(
    Combat *combat,
    Player *player)
{
    LootResults results;

    loot_generate(
        &combat->enemy->loot_table,
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

static int combat_enemy_turn(
    Combat *combat,
    Player *player)
{
    EnemyAction action =
        enemy_choose_action(
            combat->enemy,
            combat->enemy_hp,
            player->hp,
            player->max_hp);

    combat->enemy_decision =
        action + 1;

    combat->enemy_damage_dealt = 0;

    combat->enemy_defending = 0;

    // Quick Attack
    if (action == ENEMY_QUICK_ATTACK)
    {
        int damage =
            combat->enemy->damage;

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

        combat->enemy_damage_dealt =
            damage;

        player->hp -= damage;
    }

    // Heavy Attack
    else if (action == ENEMY_HEAVY_ATTACK)
    {
        int hit_chance =
            rand() % 100;

        if (hit_chance < 70)
        {
            int damage =
                combat->enemy->damage * 2;

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

            combat->enemy_damage_dealt =
                damage;

            player->hp -= damage;
        }
    }

    // Defend
    else if (action == ENEMY_DEFEND)
    {
        combat->enemy_defending = 1;
    }

    if (player->hp <= 0)
    {
        player->hp = 0;

        combat->active = 0;

        combat->player_decision = 0;
        combat->enemy_decision = 0;

        return 0;
    }

    return 1;
}

void combat_start(
    Combat *combat,
    Player *player,
    const EnemyDefinition *enemy)
{
    combat->active = 1;
    combat->loot_active = 0;

    combat->enemy = enemy;
    combat->enemy_hp = enemy->max_hp;

    player->hp = player->max_hp;

    combat->player_defending = 0;
    combat->enemy_defending = 0;

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
    printf("%s\n",
           combat->enemy->name);

    printf("HP: %d/%d\n\n",
           combat->enemy_hp,
           combat->enemy->max_hp);

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
        printf("Enemy decided: Quick Attack\n");
    }
    else if (combat->enemy_decision == 2)
    {
        printf("Enemy decided: Heavy Attack\n");
    }
    else if (combat->enemy_decision == 3)
    {
        printf("Enemy decided: Defend\n");
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

    if (combat->enemy_decision == 1 ||
        combat->enemy_decision == 2)
    {
        printf("%s dealt %d damage.\n",
               combat->enemy->name,
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

        combat->player_damage_dealt =
            player->weapons[player->current_weapon]
                .definition->damage;

        if (combat->enemy_defending)
        {
            combat->player_damage_dealt /= 2;
            combat->enemy_defending = 0;
        }

        combat->enemy_hp -=
            combat->player_damage_dealt;

        // Check if enemy died
        if (combat->enemy_hp <= 0)
        {
            combat->enemy_hp = 0;

            player_add_exp(
                player,
                combat->enemy->exp_reward);

            combat_generate_loot(
                combat,
                player);

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }

        if (!combat_enemy_turn(
                combat,
                player))
        {
            return;
        }
    }

    // Heavy Attack
    else if (input == '2')
    {
        combat->player_decision = 2;

        // 70% chance to hit
        int hit_chance =
            rand() % 100;

        if (hit_chance < 70)
        {
            combat->player_damage_dealt =
                player->weapons[player->current_weapon]
                    .definition->damage * 2;

            if (combat->enemy_defending)
            {
                combat->player_damage_dealt /= 2;
                combat->enemy_defending = 0;
            }

            combat->enemy_hp -=
                combat->player_damage_dealt;

            if (combat->enemy_hp <= 0)
            {
                combat->enemy_hp = 0;

                player_add_exp(
                    player,
                    combat->enemy->exp_reward);

                combat_generate_loot(
                    combat,
                    player);

                combat->active = 0;

                combat->player_decision = 0;
                combat->enemy_decision = 0;

                return;
            }
        }

        if (!combat_enemy_turn(
                combat,
                player))
        {
            return;
        }
    }

    // Defend
    else if (input == '3')
    {
        combat->player_decision = 3;
        combat->player_defending = 1;

        if (!combat_enemy_turn(
                combat,
                player))
        {
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