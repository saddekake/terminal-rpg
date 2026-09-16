#include <stdio.h>

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

#else

#include <termios.h>
#include <unistd.h>

#endif

#define WIDTH 19
#define HEIGHT 7

#ifdef _WIN32

void disable_raw_mode(void)
{
    // Windows doesn't need anything here.
}

void enable_raw_mode(void)
{
    // _getch() already reads a key immediately.
}

char get_input(void)
{
    int input = _getch();

    // Windows arrow keys
    if (input == 224)
    {
        input = _getch();

        switch (input)
        {
        case 72:
            return 'w'; // Up
        case 80:
            return 's'; // Down
        case 75:
            return 'a'; // Left
        case 77:
            return 'd'; // Right
        }
    }

    return (char)input;
}

#else

void disable_raw_mode(void)
{
    struct termios terminal;

    tcgetattr(STDIN_FILENO, &terminal);
    terminal.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

void enable_raw_mode(void)
{
    struct termios terminal;

    tcgetattr(STDIN_FILENO, &terminal);
    terminal.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

char get_input(void)
{
    char input;

    read(STDIN_FILENO, &input, 1);

    // Linux/macOS arrow keys
    if (input == 27)
    {
        char sequence[2];

        read(STDIN_FILENO, &sequence[0], 1);
        read(STDIN_FILENO, &sequence[1], 1);

        if (sequence[0] == '[')
        {
            switch (sequence[1])
            {
            case 'A':
                return 'w'; // Up
            case 'B':
                return 's'; // Down
            case 'C':
                return 'd'; // Right
            case 'D':
                return 'a'; // Left
            }
        }
    }

    return input;
}

#endif

// An exit connects one room to another.
struct Exit
{
    int x;
    int y;

    int destination_room;

    int destination_x;
    int destination_y;
};

// A room contains a map and its exits.
struct Room
{
    char map[HEIGHT][WIDTH + 1];

    struct Exit exits[4];
    int exit_count;
};

// Room definitions
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

        1}};

int main(void)
{

    int current_room = 0;

    int player_x = 9;
    int player_y = 2;

    int help_screen = 0;
    int control_index = 0;

    int talking_to_npc = 0;
    int npc_message_index = 0;

    int in_combat = 0;

    int player_hp = 10;
    int player_max_hp = 10;

    int enemy_hp = 7;
    int enemy_max_hp = 7;

    int player_defending = 0;

    int player_decision = 0;
    int enemy_decision = 0;

    int player_damage_dealt = 0;
    int enemy_damage_dealt = 0;

    int inventory_screen = 0;

    int player_level = 1;
    int player_exp = 0;
    int player_exp_required = 100;

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
        "Press WASD or arrow keys to move"};

    int control_count = sizeof(controls) / sizeof(controls[0]);

    // Joe's dialogue
    const char *npc_title = "Joe:";

    const char *npc_messages[] = {
        "Welcome to the game!",
        "I'm Joe.",
        "I'm an interactable friendly NPC"};

    int npc_message_count =
        sizeof(npc_messages) / sizeof(npc_messages[0]);

    enable_raw_mode();

    while (1)
    {

        // Clear terminal
        printf("\033[2J\033[H");

        // Combat screen
        if (in_combat)
        {

            printf("Skelly The Skeleton\n");
            printf("HP: %d/%d\n\n", enemy_hp, enemy_max_hp);

            printf("You\n");
            printf("HP: %d/%d\n\n", player_hp, player_max_hp);

            printf("1. Quick Attack\n");
            printf("2. Defend\n");
            printf("3. Run\n\n");

            if (player_decision == 1)
            {
                printf("You decided: Quick Attack\n");
            }
            else if (player_decision == 2)
            {
                printf("You decided: Defend\n");
            }
            else if (player_decision == 3)
            {
                printf("You decided: Run\n");
            }
            else
            {
                printf("You decided:\n");
            }

            if (enemy_decision == 1)
            {
                printf("Enemy decided: Default Attack\n");
            }
            else
            {
                printf("Enemy decided:\n");
            }

            if (player_damage_dealt > 0)
            {
                printf("You dealt %d damage.\n", player_damage_dealt);
            }

            if (enemy_decision == 1)
            {
                printf("Skelly dealt %d damage.\n", enemy_damage_dealt);
            }
        }

        // Inventory screen
        else if (inventory_screen)
        {

            printf("INVENTORY\n\n");

            printf("Level: %d\n", player_level);
            printf("EXP: %d/%d\n", player_exp, player_exp_required);
        }

        // Normal map screen
        else
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

            // Message
            if (talking_to_npc)
            {
                printf("\n%s\n", npc_title);
                printf("%s\n", npc_messages[npc_message_index]);
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
        if (in_combat)
        {

            // Quick Attack
            if (input == '1')
            {

                player_decision = 1;

                // Player attacks
                player_damage_dealt = 2;
                enemy_hp -= player_damage_dealt;

                // Check if enemy died
                if (enemy_hp <= 0)
                {
                    enemy_hp = 0;

                    // Give EXP
                    player_exp += 20;

                    // Level up
                    while (player_exp >= player_exp_required)
                    {
                        player_exp -= player_exp_required;
                        player_level++;

                        player_exp_required =
                            (int)(player_exp_required * 1.10);
                    }

                    in_combat = 0;
                    player_decision = 0;
                    enemy_decision = 0;

                    continue;
                }

                // Enemy attacks
                enemy_decision = 1;

                int damage = 1;

                if (player_defending)
                {
                    damage /= 2;
                    player_defending = 0;
                }

                enemy_damage_dealt = damage;
                player_hp -= damage;

                // Check if player died
                if (player_hp <= 0)
                {
                    player_hp = 0;

                    in_combat = 0;
                    player_decision = 0;
                    enemy_decision = 0;

                    continue;
                }
            }

            // Defend
            else if (input == '2')
            {

                player_decision = 2;
                player_defending = 1;

                // Enemy attacks
                enemy_decision = 1;

                int damage = 1;

                if (player_defending)
                {
                    damage /= 2;
                    player_defending = 0;
                }

                enemy_damage_dealt = damage;
                player_hp -= damage;

                // Check if player died
                if (player_hp <= 0)
                {
                    player_hp = 0;

                    in_combat = 0;
                    player_decision = 0;
                    enemy_decision = 0;

                    continue;
                }
            }

            // Run
            else if (input == '3')
            {

                in_combat = 0;

                player_decision = 0;
                enemy_decision = 0;
                player_damage_dealt = 0;
                enemy_damage_dealt = 0;

                continue;
            }

            continue;
        }

        // NPC dialogue
        if (talking_to_npc)
        {

            if (input == 'n' || input == 'N')
            {

                if (npc_message_index < npc_message_count - 1)
                {
                    npc_message_index++;
                }
                else
                {
                    talking_to_npc = 0;
                    npc_message_index = 0;
                }
            }

            continue;
        }

        // Inventory
        if (input == 'i' || input == 'I')
        {

            if (!in_combat && !talking_to_npc)
            {
                inventory_screen = !inventory_screen;
            }

            continue;
        }

        // Don't process other input while inventory is open
        if (inventory_screen)
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
        int new_x = player_x;
        int new_y = player_y;

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
            rooms[current_room].map[new_y][new_x];

        // Doorway
        if (destination_tile == 'I')
        {

            // Search this room's exits
            for (int i = 0; i < rooms[current_room].exit_count; i++)
            {

                struct Exit exit = rooms[current_room].exits[i];

                if (exit.x == new_x && exit.y == new_y)
                {

                    // Move to destination room
                    current_room = exit.destination_room;

                    // Place player at destination
                    player_x = exit.destination_x;
                    player_y = exit.destination_y;

                    break;
                }
            }
        }

        // Friendly NPC
        else if (destination_tile == '&')
        {

            // Start talking to the NPC
            talking_to_npc = 1;
            npc_message_index = 0;
        }

        // Enemy
        else if (destination_tile == 'X')
        {

            // Start combat
            in_combat = 1;

            // Reset combat
            player_hp = player_max_hp;
            enemy_hp = enemy_max_hp;

            player_defending = 0;

            player_decision = 0;
            enemy_decision = 0;

            player_damage_dealt = 0;
            enemy_damage_dealt = 0;
        }

        // Normal walkable tile
        else if (destination_tile != '#')
        {

            player_x = new_x;
            player_y = new_y;
        }
    }

    disable_raw_mode();

    return 0;
}