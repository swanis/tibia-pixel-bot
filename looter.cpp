#include "looter.h"

void resolve_loot_message(BitmapCapture* client, bool* loot_message) {
    for (int i = 582; i < 805; i++) {
		int rgb_one = get_rgb(client, i, 867, 8);
		int rgb_two = get_rgb(client, i, 857, 8);

		if (rgb_one == -16730896 || rgb_two == -16730896) {
			*loot_message = true;
			return;
		}
	}

	*loot_message = false;
}

void loot() {
	Sleep(200);

    key_press(VK_SHIFT);

	Sleep(75);

	click(740, 477, true);

	Sleep(75);

	click(740, 423, true);

	Sleep(75);

	click(691, 423, true);

	Sleep(75);

	click(644, 423, true);

	Sleep(75);

	click(644, 477, true);

	Sleep(75);

	click(644, 526, true);

	Sleep(75);

	click(691, 526, true);

	Sleep(75);

	click(740, 526, true);

	Sleep(200);

	key_release(VK_SHIFT);
}

void try_loot(bool loot_message, vector<unsigned short>& battle_list, int previous_battle_list_size) {
    if (!loot_message) {
        return;
    }

    if (battle_list.size() >= previous_battle_list_size) {
        return;
    }

    loot();
}
