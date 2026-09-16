#include <stdio.h>

#include "input.h"
#include "map.h"
#include "player.h"
#include "combat.h"
#include "dialogue.h"
#include "inventory.h"

Player player;
Combat combat;
Dialogue dialogue;
Inventory inventory;

int main(void)
{
    int current_room = 0;

    int help_screen = 0;
    int control_index = 0;

    // Startup messages
    const char *messages_title = "STARTUP";

    const char *messages[] = {
        "Press N to move text",
        "Welcome to my game!",
        "Press H for controls",
        "Good luck!",
        ""};

    int message_index = 0;
    int message_count = sizeof(messages) / sizeof(messages[0]);

    // Controls messages
    const char *controls_title = "HELP SCREEN";

    const char *controls[] = {
        "Press N to move text",
        "Press H for controls",
        "Press Q to quit",
        "Press WASD or arrow keys to move",
        "Press I for your inventory"};

    int control_count = sizeof(controls) / sizeof(controls[0]);

    player_init(&player);

    combat.active = 0;

    dialogue.active = 0;
    dialogue.message_index = 0;

    inventory.active = 0;

    enable_raw_mode();

    while (1)
    {
        // Clear terminal
        printf("\033[2J\033[H");

        // Combat screen
        if (combat.active)
        {
            combat_render(&combat, &player);
        }

        // Inventory screen
        else if (inventory.active)
        {
            inventory_render(&player);
        }

        // Normal map screen
        else
        {
            map_render(current_room, player.x, player.y);

            // Message
            if (dialogue.active)
            {
                dialogue_render(&dialogue);
            }
            else if (help_screen)
            {
                printf("\n%s\n", controls_title);
                printf("%s\n", controls[control_index]);
            }
            else if (messages[message_index][0] != '\0')
            {
                printf("\n%s\n", messages_title);
                printf("%s\n", messages[message_index]);
            }
        }

        // Get input
        char input = get_input();

        // Quit
        if (input == 'q' || input == 'Q')
        {
            break;
        }

        // Combat
        if (combat.active)
        {
            combat_handle_input(&combat, &player, input);

            continue;
        }

        // NPC dialogue
        if (dialogue.active)
        {
            dialogue_handle_input(&dialogue, input);

            continue;
        }

        // Inventory
        if (input == 'i' || input == 'I')
        {
            inventory_toggle(&inventory);

            continue;
        }

        // Don't process other input while inventory is open
        if (inventory.active)
        {
            continue;
        }

        // Next message
        if (input == 'n' || input == 'N')
        {
            if (help_screen)
            {
                if (control_index < control_count - 1)
                {
                    control_index++;
                }
                else
                {
                    help_screen = 0;
                    control_index = 0;
                }
            }
            else
            {
                if (message_index < message_count - 1)
                {
                    message_index++;
                }
            }

            continue;
        }

        // Controls
        if (input == 'h' || input == 'H')
        {
            help_screen = 1;
            control_index = 0;

            continue;
        }

        // Proposed new position
        int new_x = player.x;
        int new_y = player.y;

        // Movement
        if (input == 'w' || input == 'W')
        {
            new_y--;
        }
        else if (input == 's' || input == 'S')
        {
            new_y++;
        }
        else if (input == 'a' || input == 'A')
        {
            new_x--;
        }
        else if (input == 'd' || input == 'D')
        {
            new_x++;
        }

        // Make sure the position is inside the room
        if (new_x < 0 || new_x >= WIDTH ||
            new_y < 0 || new_y >= HEIGHT)
        {
            continue;
        }

        // Check what is at the proposed position
        char destination_tile =
            map_get_tile(current_room, new_x, new_y);

        // Doorway
        if (destination_tile == 'I')
        {
            int destination_room;
            int destination_x;
            int destination_y;

            if (map_find_exit(
                    current_room,
                    new_x,
                    new_y,
                    &destination_room,
                    &destination_x,
                    &destination_y))
            {
                current_room = destination_room;

                player_move(
                    &player,
                    destination_x,
                    destination_y);
            }
        }

        // Friendly NPC
        else if (destination_tile == '&')
        {
            dialogue_start(&dialogue);
        }

        // Enemy
        else if (destination_tile == 'X')
        {
            combat_start(&combat, &player);
        }

        // Normal walkable tile
        else if (destination_tile != '#')
        {
            player_move(&player, new_x, new_y);
        }
    }

    disable_raw_mode();

    return 0;
}