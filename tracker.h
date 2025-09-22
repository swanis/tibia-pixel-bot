#ifndef TRACKER_H
#define TRACKER_H

#include "essentials.h"

#include <vector>

using namespace std;

bool load_maps_and_paths(vector<BitmapCapture>& maps, vector<BitmapCapture>& paths);
void load_map_positions(vector<Point>& map_positions);
bool resolve_position(BitmapCapture* client, vector<BitmapCapture>& maps, Tracking* tracking, vector<Point>& map_positions);

#endif
