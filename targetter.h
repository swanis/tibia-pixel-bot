#ifndef TARGETTER_H
#define TARGETTER_H

#include <vector>

#include "essentials.h"

using namespace std;

void resolve_battle_list(BitmapCapture* client, vector<unsigned short>& battle_list, int* previous_battle_list_size);
void resolve_target(BitmapCapture* client, vector<unsigned short>& battle_list, unsigned short* target);
void find_target(vector<unsigned short>& battle_list, unsigned short* target);

#endif