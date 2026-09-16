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
         "#                 #",
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