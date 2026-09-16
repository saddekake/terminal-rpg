#ifndef DIALOGUE_H
#define DIALOGUE_H

typedef struct
{
    int active;
    int message_index;

} Dialogue;

void dialogue_start(Dialogue *dialogue);

void dialogue_render(const Dialogue *dialogue);

void dialogue_handle_input(Dialogue *dialogue, char input);

#endif