#include "targetter.h"

using namespace std;

#include <iostream>

void resolve_battle_list(BitmapCapture* client, vector<unsigned short>& battle_list, int* previous_battle_list_size) {
    *previous_battle_list_size = battle_list.size();
    battle_list.clear();

    int current_y = 56;
    int iterations = 1;

    for (int i = 0; i < iterations; i++) {
        int rgb = get_rgb(client, 1595, current_y, 8);

        if (rgb == -16728064 || rgb == -10436512 || rgb == -16727872 || rgb == -13619008 || rgb == -16777024 || rgb == -16777120) {
            battle_list.insert(battle_list.begin() + battle_list.size(), current_y);

            current_y += 22;
            iterations++;
        }
    }
}

void resolve_target(BitmapCapture* client, vector<unsigned short>& battle_list, unsigned short* target) {
    for (int i = 0; i < battle_list.size(); i++) {
        int rgb = get_rgb(client, 1591, battle_list[i], 8);

        if (rgb == -16776961 || rgb == -8355585) {
            *target = battle_list[i];
            return;
        }
    }

    *target = 65535;
}

void target_entry(unsigned short entry, unsigned short* target) {
    click(1620, entry, false);
    *target = entry;
    SetCursorPos(691, 477);
}

void find_target(vector<unsigned short>& battle_list, unsigned short* target) {
    if (battle_list.empty()) {
        return;
    }

    if (*target == 56) {
        return;
    }

    target_entry(battle_list[0], target);

    Sleep(100);
}
