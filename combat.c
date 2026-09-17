#include <stdio.h>
#include <stdlib.h>

#include "combat.h"

void combat_start(Combat *combat, Player *player)
{
    combat->active = 1;

    combat->enemy_hp = 7;
    combat->enemy_max_hp = 7;

    player->hp = player->max_hp;

    combat->player_defending = 0;

    combat->player_decision = 0;
    combat->enemy_decision = 0;

    combat->player_damage_dealt = 0;
    combat->enemy_damage_dealt = 0;
}

void combat_render(const Combat *combat, const Player *player)
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

    printf("You dealt %d damage.\n",
           combat->player_damage_dealt);

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
            player->weapons[player->current_weapon].damage;

        combat->enemy_hp -= combat->player_damage_dealt;

        // Check if enemy died
        if (combat->enemy_hp <= 0)
        {
            combat->enemy_hp = 0;

            // Give EXP
            player_add_exp(player, 20);

            combat->active = 0;

            combat->player_decision = 0;
            combat->enemy_decision = 0;

            return;
        }

        // Enemy attacks
        combat->enemy_decision = 1;

        int damage = 2;

        // Armor negates damage
        damage -=
            player->armor[player->current_armor].damage_negation;

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

        // Check if player died
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
            // Player attacks
            combat->player_damage_dealt =
                player->weapons[player->current_weapon].damage * 2;

            combat->enemy_hp -= combat->player_damage_dealt;

            // Check if enemy died
            if (combat->enemy_hp <= 0)
            {
                combat->enemy_hp = 0;

                // Give EXP
                player_add_exp(player, 20);

                combat->active = 0;

                combat->player_decision = 0;
                combat->enemy_decision = 0;

                return;
            }
        }

        // Enemy attacks
        combat->enemy_decision = 1;

        int damage = 2;

        // Armor negates damage
        damage -=
            player->armor[player->current_armor].damage_negation;

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

        // Check if player died
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

        // Armor negates damage
        damage -=
            player->armor[player->current_armor].damage_negation;

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

        // Check if player died
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