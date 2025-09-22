#ifndef LOOTER_H
#define LOOTER_H

#include <vector>

#include "essentials.h"

void resolve_loot_message(BitmapCapture* client, bool* loot_message);
void try_loot(bool loot_message, vector<unsigned short>& battle_list, int previous_battle_list_size);

#endif