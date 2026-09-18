#ifndef LOOT_H
#define LOOT_H

#include "items.h"
#include "player.h"

#define MAX_LOOT_ENTRIES 20
#define MAX_LOOT_RESULTS 20

typedef struct
{
    const ItemDefinition *definition;

    int weight;

    int min_quantity;
    int max_quantity;

} LootEntry;

typedef struct
{
    const LootEntry *entries;
    int entry_count;

    int rolls;

} LootTable;

typedef struct
{
    const ItemDefinition *definition;
    int quantity;

} LootResult;

typedef struct
{
    LootResult results[MAX_LOOT_RESULTS];
    int result_count;

} LootResults;

void loot_generate(
    const LootTable *table,
    const Player *player,
    LootResults *results
);

void loot_render(
    const LootResults *results
);

#endif