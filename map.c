#include <stdio.h>

#include "map.h"

struct Room rooms[2] = {

    // Room 0
    {
        {"###################",
         "#        &        #",
         "#                 #",
         "#     #######     I",
         "#                 #",
         "#                 #",
         "###################"},

        {{
            18, 3, // Door position
            1,     // Destination room
            0, 3   // Destination position
        }},

        1},

    // Room 1
    {
        {"###################",
         "#                C#",
         "#    #######      #",
         "I                 #",
         "#   X      ####   #",
         "#                 #",
         "###################"},

        {{
            0, 3, // Door position
            0,    // Destination room
            18, 3 // Destination position
        }},

        1}
    };

static const LootEntry chest_loot_entries[] = {
    {
        .definition = NULL,
        .weight = 10,
        .min_quantity = 0,
        .max_quantity = 0
    },
    {
        .definition = NULL,
        .weight = 20,
        .min_quantity = 1,
        .max_quantity = 2
    },
    {
        .definition = NULL,
        .weight = 30,
        .min_quantity = 1,
        .max_quantity = 3
    },
    {
        .definition = NULL,
        .weight = 20,
        .min_quantity = 1,
        .max_quantity = 2
    },
    {
        .definition = NULL,
        .weight = 10,
        .min_quantity = 1,
        .max_quantity = 1
    },
    {
        .definition = NULL,
        .weight = 5,
        .min_quantity = 1,
        .max_quantity = 1
    },
    {
        .definition = NULL,
        .weight = 3,
        .min_quantity = 1,
        .max_quantity = 1
    },
    {
        .definition = NULL,
        .weight = 1,
        .min_quantity = 1,
        .max_quantity = 1
    },
    {
        .definition = NULL,
        .weight = 1,
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

void map_render(int current_room, int player_x, int player_y)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == player_x && y == player_y)
            {
                printf("@");
            }
            else
            {
                printf("%c", rooms[current_room].map[y][x]);
            }
        }

        printf("\n");
    }
}

char map_get_tile(int current_room, int x, int y)
{
    if (x < 0 || x >= WIDTH ||
        y < 0 || y >= HEIGHT)
    {
        return '#';
    }

    return rooms[current_room].map[y][x];
}

int map_find_exit(
    int current_room,
    int x,
    int y,
    int *destination_room,
    int *destination_x,
    int *destination_y)
{
    for (int i = 0; i < rooms[current_room].exit_count; i++)
    {
        struct Exit exit = rooms[current_room].exits[i];

        if (exit.x == x && exit.y == y)
        {
            *destination_room = exit.destination_room;
            *destination_x = exit.destination_x;
            *destination_y = exit.destination_y;

            return 1;
        }
    }

    return 0;
}

int map_open_chest(
    int current_room,
    int x,
    int y,
    const Player *player,
    LootResults *results)
{
    if (map_get_tile(
            current_room,
            x,
            y) != 'C')
    {
        return 0;
    }

    LootEntry entries[
        sizeof(chest_loot_entries) /
        sizeof(chest_loot_entries[0])
    ];

    int chest_loot_entry_count =
        sizeof(chest_loot_entries) /
        sizeof(chest_loot_entries[0]);

    for (int i = 0;
         i < chest_loot_entry_count;
         i++)
    {
        entries[i] = chest_loot_entries[i];
    }

    entries[0].definition =
        NULL;

    entries[1].definition =
        item_find("Bronze Sword");

    entries[2].definition =
        item_find("Bronze Armor");

    entries[3].definition =
        item_find("Health Potion");

    entries[4].definition =
        item_find("Gold Coin");

    entries[5].definition =
        item_find("Quartz");

    entries[6].definition =
        item_find("Amethyst");

    entries[7].definition =
        item_find("Emerald");

    entries[8].definition =
        item_find("Ruby");

    entries[9].definition =
        item_find("Sapphire");

    LootTable table = {
        .entries = entries,
        .entry_count =
            sizeof(entries) /
            sizeof(entries[0]),
        .rolls = 15
    };

    loot_generate(
        &table,
        player,
        results);

    rooms[current_room].map[y][x] = ' ';

    return 1;
}