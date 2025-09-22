#include "tracker.h"

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

bool load_maps_and_paths(vector<BitmapCapture>& maps, vector<BitmapCapture>& paths) {
    for (int i = 0; i < 16; i++) {
        BitmapCapture floorMap;
        BitmapCapture floorPath;

        string sMap = i < 10 ? string("maps/floor-0") + to_string(i) + "-map.png" : string("maps/floor-") + to_string(i) + "-map.png";
        string sPath = i < 10 ? string("maps/floor-0") + to_string(i) + "-path.png" : string("maps/floor-") + to_string(i) + "-path.png";

        wstring wideStringMap = wstring(sMap.begin(), sMap.end());
        wstring wideStringPath = wstring(sPath.begin(), sPath.end());

        if (capture_image(wideStringMap.c_str(), &floorMap)) {
            maps.emplace_back(floorMap);
        } else {
            cout << "failed to capture image " << sMap << endl;
            return false;
        }

        if (capture_image(wideStringPath.c_str(), &floorPath)) {
            paths.emplace_back(floorPath);
        } else {
            cout << "failed to capture image " << sPath << endl;
            return false;
        }
    }

    return true;
}

void load_map_positions(vector<Point>& map_positions) {
    vector<unsigned short> x;
    vector<unsigned short> y;

    x.push_back(0);
    y.push_back(0);

    x.push_back(104);
    y.push_back(108);

    x.push_back(0);
    y.push_back(108);

    x.push_back(104);
    y.push_back(0);

    x.push_back(30);
    y.push_back(30);

    x.push_back(70);
    y.push_back(70);

    x.push_back(30);
    y.push_back(70);

    x.push_back(70);
    y.push_back(30);

    x.push_back(50);
    y.push_back(50);

    x.push_back(57);
    y.push_back(57);

    x.push_back(50);
    y.push_back(57);

    x.push_back(57);
    y.push_back(50);

    for (int i = 0; i < x.size(); i++) {
        Point point;

        point.x = x[i];
        point.y = y[i];

        map_positions.push_back(point);
    }
}

bool at_position(BitmapCapture* client, BitmapCapture* map, int x, int y, int floor, Tracking* tracking, vector<Point>& map_positions) {
    auto impossible = [&] {
        for (Point point : map_positions) {
            if (x + point.x > 2559 || y + point.y > 2047) {
                return true;
            }

            int map_rgb = get_rgb(map, x + point.x, y + point.y, 0);
            int minimap_rgb = get_rgb(client, 1753 + point.x, 136 - point.y, 8);

            //perhaps a small mistake factor here too if map is modified
            if (map_rgb != minimap_rgb) {
                return true;
            }
        }

        return false;
    }();

    if (impossible) {
        return false;
    }

    auto broke = [&] {
        int misses = 0;

        for (int minimap_x = 0; minimap_x < 104; minimap_x++) {
            for (int minimap_y = 0; minimap_y < 108; minimap_y++) {
                int minimap_rgb = get_rgb(client, 1753 + minimap_x, 136 - minimap_y, 8);

                if (minimap_rgb == -1) {
                    continue;
                }

                int map_rgb = get_rgb(map, x + minimap_x, y + minimap_y, 0);

                if (minimap_rgb != map_rgb) {
                    if (++misses > 50) {
                        return true;
                    }
                }
            }
        }

        return false;
    }();

    if (!broke) {
        unsigned short position_x = 31744 + x + 53;
        unsigned short position_z = floor;
        unsigned short position_y = 33023 - y - 54;

        if (tracking->position->x != position_x || tracking->position->z != position_z || tracking->position->y != position_y) {
            tracking->last_move = chrono::steady_clock::now();
        }

        tracking->position->x = position_x;
        tracking->position->z = position_z;
        tracking->position->y = position_y;

        return true;
    }

    return false;
}

bool resolve_position(BitmapCapture* client, vector<BitmapCapture>& maps, Tracking* tracking, vector<Point>& map_positions) {
    int floor = tracking->position->z;

    for (int i = 0; i < 16; i++) {
        if (i != 0) {
            if (i % 2 == 0) {
                floor = tracking->position->z - (i / 2);
            } else {
                floor = tracking->position->z + (i / 2) + 1;
            }
        }

        if (floor < 0) {
            floor = 15 - (((i / 2) - 1) - (tracking->position->z - 0));
        } else if (floor > 15) {
            floor = 0 + ((i / 2) - (15 - tracking->position->z));
        }

        BitmapCapture map = maps[floor];

        //check 5x5 area around client's last position (only necessary when on same floor or one floor up/down)
        if (i < 3) {
            for (int x = -2; x < 3; x++) {
                for (int y = -2; y < 3; y++) {
                    if (at_position(client, &map, ((tracking->position->x - 31744 - 53) + x), ((33023 - 54 - tracking->position->y) + y), floor, tracking, map_positions)) {
                        return true;
                    }
                }
            }
        }

        //check full map
        for (int x = 0; x < 2560; x++) {
            for (int y = 0; y < 2048; y++) {
                if (at_position(client, &map, x, y, floor, tracking, map_positions)) {
                    return true;
                }
            }
        }

        //avoid weird bug with floor change
        if (i == 0) {
            DeleteObject(client->hbm);
            Sleep(250);
            capture_client(client);
        }
    }

    return false;
}
