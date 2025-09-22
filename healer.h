#ifndef HEALER_H
#define HEALER_H

//#include "essentials.h"

//bool resolve_health(BitmapCapture* client, double* health);

#include <string>
#include <atomic>

using namespace std;

class PotionHealerEntry {
private:
	std::string name_;
	int key_;
	bool healMana_;
	int health_;
	int mana_;
public:
	string getName();
	int getKey();
	bool isHealMana();
	int getHealth();
	int getMana();

	PotionHealerEntry(string name, int key, int health, int mana, bool healMana);
};

class SpellHealerEntry {
private:
	std::string name_;
	int key_;
	int healthUnder_;
	int manaOver_;
public:
	std::string getName();
	int getKey();
	int getHealthUnder();
	int getManaOver();

	SpellHealerEntry(string name, int key, int healthUnder, int manaOver);
};

void start_healer();
extern atomic<bool> stop_healer;

#endif
