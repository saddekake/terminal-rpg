#ifndef MAP_H
#define MAP_H

#include "loot.h"

#define WIDTH 19
#define HEIGHT 7

struct Exit
{
    int x;
    int y;

    int destination_room;

    int destination_x;
    int destination_y;
};

struct Room
{
    char map[HEIGHT][WIDTH + 1];

    struct Exit exits[4];
    int exit_count;
};

extern struct Room rooms[2];

void map_render(int current_room, int player_x, int player_y);

char map_get_tile(int current_room, int x, int y);

int map_find_exit(
    int current_room,
    int x,
    int y,
    int *destination_room,
    int *destination_x,
    int *destination_y
);

int map_open_chest(
    int current_room,
    int x,
    int y,
    const Player *player,
    LootResults *results
);

#endif