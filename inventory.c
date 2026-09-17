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

    if (mode == INVENTORY_NORMAL)
    {
        inventory->selected_category = 0;
        inventory->selected_item = 0;
    }
    else
    {
        // Combat inventory starts on Items
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
           player->weapons[player->current_weapon].name);

    printf("  Attack: +%d\n\n",
           player->weapons[player->current_weapon].damage);

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
           player->armor[player->current_armor].name);

    printf("  Damage Negation: %d\n\n",
           player->armor[player->current_armor].damage_negation);

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

    for (int i = 0; i < player->item_count; i++)
    {
        if (inventory->mode == INVENTORY_COMBAT &&
            inventory->selected_item == i)
        {
            printf("> %s (x%d)\n",
                   player->items[i].name,
                   player->items[i].quantity);
        }
        else if (inventory->mode == INVENTORY_NORMAL &&
                 inventory->selected_category == 2 &&
                 inventory->selected_item == i)
        {
            printf("> %s (x%d)\n",
                   player->items[i].name,
                   player->items[i].quantity);
        }
        else
        {
            printf("  %s (x%d)\n",
                   player->items[i].name,
                   player->items[i].quantity);
        }
    }

    printf("\n");

    // Normal inventory controls
    if (inventory->mode == INVENTORY_NORMAL)
    {
        if (inventory->selected_category == 0)
        {
            printf("Selected: %s\n",
                   player->weapons[player->current_weapon].name);

            printf("B/N: Select    E: Cycle Weapon    I: Close\n");
        }
        else if (inventory->selected_category == 1)
        {
            printf("Selected: %s\n",
                   player->armor[player->current_armor].name);

            printf("B/N: Select    E: Cycle Armor    I: Close\n");
        }
        else if (player->item_count > 0)
        {
            printf("Selected: %s\n",
                   player->items[inventory->selected_item].name);

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
                   player->items[inventory->selected_item].name);

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
            }
            else
            {
                inventory->selected_item = 0;
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
            }
            else
            {
                inventory->selected_item =
                    player->item_count - 1;
            }
        }

        // Use item
        else if (input == 'e' || input == 'E')
        {
            if (player->item_count == 0)
            {
                return;
            }

            Item *item =
                &player->items[inventory->selected_item];

            if (item->quantity <= 0)
            {
                return;
            }

            if (item->type == ITEM_HEALTH_POTION)
            {
                player->hp += item->healing;

                if (player->hp > player->max_hp)
                {
                    player->hp = player->max_hp;
                }

                item->quantity--;
            }

            else if (item->type == ITEM_PLACEHOLDER_POTION)
            {
                // Does nothing for now.
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
                }
                else if (inventory->selected_item >=
                         player->item_count)
                {
                    inventory->selected_item =
                        player->item_count - 1;
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
                return;
            }

            if (inventory->selected_item <
                player->item_count - 1)
            {
                inventory->selected_item++;
            }
            else
            {
                inventory->selected_category = 0;
                inventory->selected_item = 0;
            }
        }
        else
        {
            inventory->selected_category++;
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
            }
            else
            {
                inventory->selected_category = 1;
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

        // Items
        // Items cannot be used outside combat.
    }
}