#include "input.h"

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

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

#include <termios.h>
#include <unistd.h>

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