#include <stdio.h>
#include <stdlib.h>

#include "loot.h"

static int player_has_item(
    const Player *player,
    const ItemDefinition *definition)
{
    for (int i = 0;
         i < player->weapon_count;
         i++)
    {
        if (player->weapons[i].definition ==
            definition)
        {
            return 1;
        }
    }

    for (int i = 0;
         i < player->armor_count;
         i++)
    {
        if (player->armor[i].definition ==
            definition)
        {
            return 1;
        }
    }

    for (int i = 0;
         i < player->item_count;
         i++)
    {
        if (player->items[i].definition ==
            definition)
        {
            return 1;
        }
    }

    return 0;
}

static int loot_already_contains(
    const LootResults *results,
    const ItemDefinition *definition)
{
    for (int i = 0;
         i < results->result_count;
         i++)
    {
        if (results->results[i].definition ==
            definition)
        {
            return 1;
        }
    }

    return 0;
}

void loot_generate(
    const LootTable *table,
    const Player *player,
    LootResults *results)
{
    results->result_count = 0;

    for (int roll = 0;
         roll < table->rolls;
         roll++)
    {
        int total_weight = 0;

        // Calculate the weight of all eligible entries.
        for (int i = 0;
             i < table->entry_count;
             i++)
        {
            const ItemDefinition *definition =
                table->entries[i].definition;

            if (definition != NULL &&
                definition->unique)
            {
                if (player_has_item(
                        player,
                        definition) ||
                    loot_already_contains(
                        results,
                        definition))
                {
                    continue;
                }
            }

            total_weight +=
                table->entries[i].weight;
        }

        if (total_weight <= 0)
        {
            continue;
        }

        int random_value =
            rand() % total_weight;

        const LootEntry *selected_entry = NULL;

        // Select an eligible entry.
        for (int i = 0;
             i < table->entry_count;
             i++)
        {
            const ItemDefinition *definition =
                table->entries[i].definition;

            if (definition != NULL &&
                definition->unique)
            {
                if (player_has_item(
                        player,
                        definition) ||
                    loot_already_contains(
                        results,
                        definition))
                {
                    continue;
                }
            }

            if (random_value <
                table->entries[i].weight)
            {
                selected_entry =
                    &table->entries[i];

                break;
            }

            random_value -=
                table->entries[i].weight;
        }

        if (selected_entry == NULL)
        {
            continue;
        }

        if (selected_entry->definition == NULL)
        {
            continue;
        }

        int quantity =
            selected_entry->min_quantity;

        if (selected_entry->definition != NULL &&
            selected_entry->definition->unique)
        {
            quantity = 1;
        }
        else if (selected_entry->max_quantity >
                 selected_entry->min_quantity)
        {
            quantity +=
                rand() %
                (selected_entry->max_quantity -
                 selected_entry->min_quantity + 1);
        }

        int found = 0;

        for (int i = 0;
             i < results->result_count;
             i++)
        {
            if (results->results[i].definition ==
                selected_entry->definition)
            {
                results->results[i].quantity +=
                    quantity;

                found = 1;

                break;
            }
        }

        if (!found &&
            results->result_count < MAX_LOOT_RESULTS)
        {
            results->results[
                results->result_count
            ].definition =
                selected_entry->definition;

            results->results[
                results->result_count
            ].quantity = quantity;

            results->result_count++;
        }
    }
}

void loot_render(
    const LootResults *results)
{
    printf("Loot:\n");

    if (results->result_count == 0)
    {
        printf("Nothing\n");
    }
    else
    {
        for (int i = 0;
             i < results->result_count;
             i++)
        {
            printf("%s x%d\n",
                   results->results[i].definition->name,
                   results->results[i].quantity);
        }
    }

    printf("\nPress N to continue\n");
}