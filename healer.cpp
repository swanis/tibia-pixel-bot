#include "healer.h"

#include <iostream>
#include <vector>

#include "essentials.h"

using namespace std;

std::string SpellHealerEntry::getName() {
	return name_;
}

int SpellHealerEntry::getKey() {
	return key_;
}

int SpellHealerEntry::getHealthUnder() {
	return healthUnder_;
}

int SpellHealerEntry::getManaOver() {
	return manaOver_;
}

SpellHealerEntry::SpellHealerEntry(string name, int key, int healthUnder, int manaOver) {
	name_ = name;
	key_ = key;
	healthUnder_ = healthUnder;
	manaOver_ = manaOver;
}

string PotionHealerEntry::getName() {
	return name_;
}

int PotionHealerEntry::getKey() {
	return key_;
}

bool PotionHealerEntry::isHealMana() {
	return healMana_;
}

int PotionHealerEntry::getHealth() {
	return health_;
}

int PotionHealerEntry::getMana() {
	return mana_;
}

PotionHealerEntry::PotionHealerEntry(string name, int key, int health, int mana, bool healMana) {
	name_ = name;
	key_ = key;
	health_ = health;
	mana_ = mana;
	healMana_ = healMana;
}

void resolve_health(BitmapCapture* client, double* health) {
    int filled = 0;

    for (int i = 1767; i < 1860; i++) {
        int rgb = get_rgb(client, i, 307, 8);

        if (rgb == -10460992 || rgb == -12566336 || rgb == -13224292 || rgb == -13883571) {
            filled++;
        }
    }

    double fraction = (double) filled / 93;
    *health = fraction * 100;
}

void resolve_mana(BitmapCapture* client, double* mana) {
    int filled = 0;

    for (int i = 1767; i < 1860; i++) {
        int rgb = get_rgb(client, i, 320, 8);

		if (rgb == -4169630 || rgb == -4177853 || rgb == -6539720 || rgb == -11262170) {
			filled++;
		}
    }

    double fraction = (double) filled / 93;
    *mana = fraction * 100;
}

bool has_heal_cooldown_pixel(BitmapCapture* client) {
	int rgb = get_rgb(client, 42, 938, 8);

	return rgb == -924472;
}

vector<PotionHealerEntry> potion_healer_entries;
chrono::steady_clock::time_point potion_cooldown = chrono::steady_clock::now();

void potion_heal(double* health, double* mana) {
    chrono::steady_clock::time_point now = chrono::steady_clock::now();

    if (chrono::duration_cast<chrono::milliseconds>(now - potion_cooldown).count() < 250) {
        return;
    }

    for (PotionHealerEntry entry : potion_healer_entries) {
        if (entry.isHealMana()) {
            if (*mana < entry.getMana()) {
                key_press(entry.getKey());
                Sleep(50);
                key_release(entry.getKey());

                potion_cooldown = chrono::steady_clock::now();
                break;
            }
        } else {
            if (*health < entry.getHealth()) {
                key_press(entry.getKey());
                Sleep(50);
                key_release(entry.getKey());

                potion_cooldown = chrono::steady_clock::now();
                break;
            }
        }
    }
}

void load_potion() {
	potion_healer_entries.push_back(PotionHealerEntry("Health", 118, 50, 0, false));
	potion_healer_entries.push_back(PotionHealerEntry("Mana", 119, 0, 80, true));
}

vector<SpellHealerEntry> spell_healer_entries;
chrono::steady_clock::time_point spell_cooldown = chrono::steady_clock::now();

void spell_heal(BitmapCapture* client, double* health, double* mana) {
    chrono::steady_clock::time_point now = chrono::steady_clock::now();

    if (chrono::duration_cast<chrono::milliseconds>(now - spell_cooldown).count() < 125) {
        return;
    }

    if (has_heal_cooldown_pixel(client)) {
        return;
    }

    for (SpellHealerEntry entry : spell_healer_entries) {
        if (*health < entry.getHealthUnder() && *mana > entry.getManaOver()) {
            key_press(entry.getKey());
            Sleep(50);
            key_release(entry.getKey());

            spell_cooldown = chrono::steady_clock::now();
            break;
        }
    }
}

void load_spell() {
    //prev was 90 - 20
    spell_healer_entries.push_back(SpellHealerEntry("Exura Ico", 116, 95, 10));
}

atomic<bool> stop_healer {false};

void start_healer() {
    BitmapCapture client;

    double health;
    double mana;

    load_potion();
    load_spell();

    while (!stop_healer.load()) {
        if (!capture_client(&client)) {
            cout << "H - Could not capture" << endl;
            continue;
        }

        resolve_health(&client, &health);
        resolve_mana(&client, &mana);

        potion_heal(&health, &mana);
        spell_heal(&client, &health, &mana);

        DeleteObject(client.hbm);

        Sleep(50);
    }
}
