#include <stdio.h>

#include "inventory.h"

void inventory_toggle(Inventory *inventory)
{
    if (inventory->active)
    {
        inventory_close(inventory);
    }
    else
    {
        inventory_open(
            inventory,
            INVENTORY_NORMAL);
    }
}

void inventory_open(
    Inventory *inventory,
    InventoryMode mode)
{
    inventory->active = 1;
    inventory->mode = mode;

    inventory->item_scroll = 0;

    if (mode == INVENTORY_NORMAL)
    {
        inventory->selected_category = 0;
        inventory->selected_item = 0;
    }
    else
    {
        inventory->selected_category = 2;
        inventory->selected_item = 0;
    }
}

void inventory_close(Inventory *inventory)
{
    inventory->active = 0;
}

void inventory_render(
    const Inventory *inventory,
    const Player *player)
{
    printf("INVENTORY\n\n");

    printf("Level: %d\n", player->level);
    printf("EXP: %d/%d\n\n",
           player->exp,
           player->exp_required);

    // Weapon
    if (inventory->mode == INVENTORY_NORMAL &&
        inventory->selected_category == 0)
    {
        printf("> Weapon\n");
    }
    else
    {
        printf("  Weapon\n");
    }

    printf("  %s\n",
           player->weapons[player->current_weapon]
               .definition->name);

    printf("  Attack: +%d\n\n",
           player->weapons[player->current_weapon]
               .definition->damage);

    // Armor
    if (inventory->mode == INVENTORY_NORMAL &&
        inventory->selected_category == 1)
    {
        printf("> Armor\n");
    }
    else
    {
        printf("  Armor\n");
    }

    printf("  %s\n",
           player->armor[player->current_armor]
               .definition->name);

    printf("  Damage Negation: %d\n\n",
           player->armor[player->current_armor]
               .definition->damage_negation);

    // Items
    if (inventory->mode == INVENTORY_NORMAL)
    {
        printf("Items\n");
    }
    else
    {
        if (player->item_count > 0)
        {
            printf("> Items\n");
        }
        else
        {
            printf("  Items\n");
        }
    }

    // Visible item window
    int visible_end =
        inventory->item_scroll +
        INVENTORY_VISIBLE_ITEMS;

    if (visible_end > player->item_count)
    {
        visible_end = player->item_count;
    }

    // More items above
    if (inventory->item_scroll > 0)
    {
        printf("  ^ More above\n");
    }

    for (int i = inventory->item_scroll;
         i < visible_end;
         i++)
    {
        if (inventory->mode == INVENTORY_COMBAT &&
            inventory->selected_item == i)
        {
            printf("> %s (x%d)\n",
                   player->items[i].definition->name,
                   player->items[i].quantity);
        }
        else if (inventory->mode == INVENTORY_NORMAL &&
                 inventory->selected_category == 2 &&
                 inventory->selected_item == i)
        {
            printf("> %s (x%d)\n",
                   player->items[i].definition->name,
                   player->items[i].quantity);
        }
        else
        {
            printf("  %s (x%d)\n",
                   player->items[i].definition->name,
                   player->items[i].quantity);
        }
    }

    // More items below
    if (visible_end < player->item_count)
    {
        printf("  v More below\n");
    }

    printf("\n");

    // Normal inventory controls
    if (inventory->mode == INVENTORY_NORMAL)
    {
        if (inventory->selected_category == 0)
        {
            printf("Selected: %s\n",
                   player->weapons[player->current_weapon]
                       .definition->name);

            printf("B/N: Select    E: Cycle Weapon    I: Close\n");
        }
        else if (inventory->selected_category == 1)
        {
            printf("Selected: %s\n",
                   player->armor[player->current_armor]
                       .definition->name);

            printf("B/N: Select    E: Cycle Armor    I: Close\n");
        }
        else if (player->item_count > 0)
        {
            printf("Selected: %s\n",
                   player->items[inventory->selected_item]
                       .definition->name);

            printf("B/N: Select    E: Cannot Use    I: Close\n");
        }
        else
        {
            printf("Selected: None\n");

            printf("B/N: Select    I: Close\n");
        }
    }

    // Combat inventory controls
    else
    {
        if (player->item_count > 0)
        {
            printf("Selected: %s\n",
                   player->items[inventory->selected_item]
                       .definition->name);

            printf("B/N: Select    E: Use Item    I: Return\n");
        }
        else
        {
            printf("Selected: None\n");

            printf("B/N: Select    I: Return\n");
        }
    }
}

void inventory_handle_input(
    Inventory *inventory,
    Player *player,
    char input)
{
    // Combat inventory
    if (inventory->mode == INVENTORY_COMBAT)
    {
        // Next item
        if (input == 'n' || input == 'N')
        {
            if (player->item_count == 0)
            {
                return;
            }

            if (inventory->selected_item <
                player->item_count - 1)
            {
                inventory->selected_item++;

                // Scroll down whhn selection reaches the bottom of the visible window
                if (inventory->selected_item >=
                    inventory->item_scroll +
                        INVENTORY_VISIBLE_ITEMS)
                {
                    inventory->item_scroll++;
                }
            }
        }

        // Previous item
        else if (input == 'b' || input == 'B')
        {
            if (player->item_count == 0)
            {
                return;
            }

            if (inventory->selected_item > 0)
            {
                inventory->selected_item--;

                // Scroll up when selection reaches the top of the visible window
                if (inventory->selected_item <
                    inventory->item_scroll)
                {
                    inventory->item_scroll--;
                }
            }
        }

        // Use item
        else if (input == 'e' || input == 'E')
        {
            if (player->item_count == 0)
            {
                return;
            }

            InventoryItem *item =
                &player->items[inventory->selected_item];

            if (item->quantity <= 0)
            {
                return;
            }

            if (item->definition->category ==
                ITEM_CONSUMABLE)
            {
                player->hp +=
                    item->definition->healing;

                if (player->hp > player->max_hp)
                {
                    player->hp = player->max_hp;
                }

                item->quantity--;
            }

            // Remove empty item
            if (item->quantity <= 0)
            {
                for (int i = inventory->selected_item;
                     i < player->item_count - 1;
                     i++)
                {
                    player->items[i] =
                        player->items[i + 1];
                }

                player->item_count--;

                if (player->item_count == 0)
                {
                    inventory->selected_item = 0;
                    inventory->item_scroll = 0;
                }
                else
                {
                    if (inventory->selected_item >=
                        player->item_count)
                    {
                        inventory->selected_item =
                            player->item_count - 1;
                    }

                    // Make sure the selected item is still inside the visible window
                    if (inventory->selected_item <
                        inventory->item_scroll)
                    {
                        inventory->item_scroll =
                            inventory->selected_item;
                    }

                    if (inventory->item_scroll +
                            INVENTORY_VISIBLE_ITEMS >
                        player->item_count)
                    {
                        inventory->item_scroll =
                            player->item_count -
                            INVENTORY_VISIBLE_ITEMS;

                        if (inventory->item_scroll < 0)
                        {
                            inventory->item_scroll = 0;
                        }
                    }
                }
            }
        }

        return;
    }

    // Normal inventory
    // Next
    if (input == 'n' || input == 'N')
    {
        if (inventory->selected_category == 2)
        {
            if (player->item_count == 0)
            {
                inventory->selected_category = 0;
                inventory->selected_item = 0;
                inventory->item_scroll = 0;

                return;
            }

            if (inventory->selected_item <
                player->item_count - 1)
            {
                inventory->selected_item++;

                // Scroll down when selection reaches the bottom of the visible window
                if (inventory->selected_item >=
                    inventory->item_scroll +
                        INVENTORY_VISIBLE_ITEMS)
                {
                    inventory->item_scroll++;
                }
            }
            else
            {
                // At the final item, move to the next category
                inventory->selected_category = 0;
                inventory->selected_item = 0;
                inventory->item_scroll = 0;
            }
        }
        else
        {
            inventory->selected_category++;

            // Entering Items starts at the beginning
            if (inventory->selected_category == 2)
            {
                inventory->selected_item = 0;
                inventory->item_scroll = 0;
            }
        }
    }

    // Previous
    else if (input == 'b' || input == 'B')
    {
        if (inventory->selected_category == 2)
        {
            if (inventory->selected_item > 0)
            {
                inventory->selected_item--;

                // Scroll up when selection reaches the top of the visible window
                if (inventory->selected_item <
                    inventory->item_scroll)
                {
                    inventory->item_scroll--;
                }
            }
            else
            {
                // At the first item, move to the previous category
                inventory->selected_category = 1;
                inventory->selected_item = 0;
                inventory->item_scroll = 0;
            }
        }
        else if (inventory->selected_category > 0)
        {
            inventory->selected_category--;
        }
        else
        {
            if (player->item_count > 0)
            {
                inventory->selected_category = 2;
                inventory->selected_item =
                    player->item_count - 1;

                // Position the window at the end of the item list
                inventory->item_scroll =
                    player->item_count -
                    INVENTORY_VISIBLE_ITEMS;

                if (inventory->item_scroll < 0)
                {
                    inventory->item_scroll = 0;
                }
            }
            else
            {
                inventory->selected_category = 1;
            }
        }
    }

    // Equip
    else if (input == 'e' || input == 'E')
    {
        // Weapon
        if (inventory->selected_category == 0)
        {
            if (player->weapon_count > 0)
            {
                player->current_weapon++;

                if (player->current_weapon >=
                    player->weapon_count)
                {
                    player->current_weapon = 0;
                }
            }
        }

        // Armor
        else if (inventory->selected_category == 1)
        {
            if (player->armor_count > 0)
            {
                player->current_armor++;

                if (player->current_armor >=
                    player->armor_count)
                {
                    player->current_armor = 0;
                }
            }
        }
    }
}