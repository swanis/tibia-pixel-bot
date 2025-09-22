#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <windows.h>
#include <gdiplus.h>

#include "essentials.h"
#include "healer.h"
#include "tracker.h"
#include "cavebotter.h"
#include "targetter.h"
#include "looter.h"

using namespace std;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);

	//Sleep(1000);

	vector<BitmapCapture> maps;
	vector<BitmapCapture> paths;

    Gdiplus::GdiplusStartupInput gpStartupInput;
    ULONG_PTR gpToken;
    Gdiplus::GdiplusStartup(&gpToken, &gpStartupInput, NULL);

	if (!load_maps_and_paths(maps, paths)) {
		cout << "could not load maps and paths" << endl;
		return 0;
	}

    Gdiplus::GdiplusShutdown(gpToken);

	BitmapCapture client;
	double health;
	Position position;

	position.x = 0;
	position.z = 7;
	position.y = 0;

	Tracking tracking;

	tracking.position = &position;
	tracking.last_move = chrono::steady_clock::now();

	vector<Point> map_positions;

	load_map_positions(map_positions);

	thread healer(start_healer);

	load_waypoints();

	vector<unsigned short> battle_list;
	int previous_battle_list_size;
	unsigned short target;
	bool loot_message;

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	while (true) {
		if (!capture_client(&client)) {
			cout << GetLastError() << endl;
			cout << "Could not capture" << endl;
			continue;
		}

		if ((GetKeyState(VK_DELETE) & 0x100) != 0) {
			return 0;
		}

		resolve_battle_list(&client, battle_list, &previous_battle_list_size);
		resolve_loot_message(&client, &loot_message);

		try_loot(loot_message, battle_list, previous_battle_list_size);

		if (get_rgb(&client, 1902, 174, 8) != -10682532) {
		    click(1902, 174, false);
		}

		resolve_target(&client, battle_list, &target);

		find_target(battle_list, &target);

		resolve_position(&client, maps, &tracking, map_positions);

		if (target == 65535 || chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - tracking.last_move).count() > 45000) {
		    cavebot(&tracking);
		}

		DeleteObject(client.hbm);

		/*chrono::steady_clock::time_point end = chrono::steady_clock::now();
		cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;
		cout << position.x << ":" << position.z << ":" << position.y << endl;

		begin = chrono::steady_clock::now();*/

		Sleep(50);
	}

	delete_waypoints();

	for (int i = 0; i < maps.size(); i++) {
		DeleteObject(maps[i].hbm);
		DeleteObject(paths[i].hbm);
	}

	stop_healer.store(true);

	healer.join();

	return 0;
}
