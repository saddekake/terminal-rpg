#include <stdio.h>

#include "dialogue.h"

static const char *npc_title = "Joe:";

static const char *npc_messages[] = {
    "Welcome to the game!",
    "I'm Joe.",
    "I'm an interactable friendly NPC"};

static const int npc_message_count =
    sizeof(npc_messages) / sizeof(npc_messages[0]);

void dialogue_start(Dialogue *dialogue)
{
    dialogue->active = 1;
    dialogue->message_index = 0;
}

void dialogue_render(const Dialogue *dialogue)
{
    printf("\n%s\n", npc_title);
    printf("%s\n", npc_messages[dialogue->message_index]);
}

void dialogue_handle_input(Dialogue *dialogue, char input)
{
    if (input == 'n' || input == 'N')
    {
        if (dialogue->message_index < npc_message_count - 1)
        {
            dialogue->message_index++;
        }
        else
        {
            dialogue->active = 0;
            dialogue->message_index = 0;
        }
    }
}