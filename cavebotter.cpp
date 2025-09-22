#include "cavebotter.h"

#include <iostream>
#include <vector>

using namespace std;

void walk_to_goal(int x, int y, Position* position) {
    click(1806 - (position->x - x), 82 - (position->y - y), false);
}

int x_of(Direction direction) {
    int x;

    switch (direction) {
        case EAST: x = 740; break;
        case NORTH_EAST: x = 740; break;
        case NORTH: x = 691; break;
        case NORTH_WEST: x = 644; break;
        case WEST: x = 644; break;
        case SOUTH_WEST: x = 644; break;
        case SOUTH: x = 691; break;
        case SOUTH_EAST: x = 740; break;
        case CENTER: x = 691; break;
        default: break;
    }

    return x;
}

int y_of(Direction direction) {
    int y;

    switch (direction) {
        case EAST: y = 477; break;
        case NORTH_EAST: y = 423; break;
        case NORTH: y = 423; break;
        case NORTH_WEST: y = 423; break;
        case WEST: y = 477; break;
        case SOUTH_WEST: y = 526; break;
        case SOUTH: y = 526; break;
        case SOUTH_EAST: y = 526; break;
        case CENTER: y = 477; break;
        default: break;
    }

    return y;
}

bool stand(Waypoint* stand, Tracking* tracking) {
    if (tracking->position->x == stand->getX() && tracking->position->y == stand->getY()) {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();

        return chrono::duration_cast<chrono::milliseconds>(now - tracking->last_move).count() > 500;
    }

    chrono::steady_clock::time_point now = chrono::steady_clock::now();

    if (chrono::duration_cast<chrono::milliseconds>(now - tracking->last_move).count() < 500) {
        return false;
    }

    walk_to_goal(stand->getX(), stand->getY(), tracking->position);
    tracking->last_move = chrono::steady_clock::now();

    return false;
}

bool node(Waypoint* node, Tracking* tracking) {
    if (abs(tracking->position->x - node->getX()) < 4 && abs(tracking->position->y - node->getY()) < 4) {
        return true;
    }

    chrono::steady_clock::time_point now = chrono::steady_clock::now();

    if (chrono::duration_cast<chrono::milliseconds>(now - tracking->last_move).count() < 500) {
        return false;
    }

    walk_to_goal(node->getX(), node->getY(), tracking->position);
    tracking->last_move = chrono::steady_clock::now();

    return false;
}

bool walk(WalkWaypoint* walk, Tracking* tracking) {
    if (!stand(walk, tracking)) {
        return false;
    }

    int key;

    switch (walk->getDirection()) {
        case EAST: key = 39; break;
        case NORTH_EAST: key = 33; break;
        case NORTH: key = 38; break;
        case NORTH_WEST: key = 36; break;
        case WEST: key = 37; break;
        case SOUTH_WEST: key = 35; break;
        case SOUTH: key = 40; break;
        case SOUTH_EAST: key = 34; break;
        default: break;
    }

    key_press(key);
    key_release(key);

    Sleep(200);
    return true;
}

bool use(UseWaypoint* use, Tracking* tracking) {
    if (!stand(use, tracking)) {
        return false;
    }

    int x = x_of(use->getDirection());
    int y = y_of(use->getDirection());

    click(x, y, true);

    Sleep(200);
    return true;
}

bool rope(RopeWaypoint* rope,  Tracking* tracking) {
    if (!stand(rope, tracking)) {
        return false;
    }

    int x = x_of(rope->getDirection());
    int y = y_of(rope->getDirection());

    key_press(112);
    Sleep(100);
    key_release(112);
    Sleep(200);
    click(x, y, false);

    Sleep(200);
    return true;
}

bool shovel(ShovelWaypoint* shovel, Tracking* tracking) {
    if (!stand(shovel, tracking)) {
        return false;
    }

    int x = x_of(shovel->getDirection());
    int y = y_of(shovel->getDirection());

    key_press(113);
    Sleep(100);
    key_release(113);
    Sleep(200);
    click(x, y, false);

    Sleep(200);
    return true;
}

bool handle(Waypoint* waypoint, Tracking* tracking) {
    return [&] {
        switch (waypoint->getType()) {
            case STAND: return stand(waypoint, tracking);
            case NODE: return node(waypoint, tracking);
            case WALK: return walk(dynamic_cast<WalkWaypoint*>(waypoint), tracking);
            case USE: return use(dynamic_cast<UseWaypoint*>(waypoint), tracking);
            case ROPE: return rope(dynamic_cast<RopeWaypoint*>(waypoint), tracking);
            case SHOVEL: return shovel(dynamic_cast<ShovelWaypoint*>(waypoint), tracking);
            default: break;
        }

        return false;
    }();
}

vector<Waypoint*> waypoints;

void load_waypoints() {
    //BOTTOM FLOOR
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32797, 11, 31945));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32777, 11, 31946));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32778, 11, 31954));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32780, 11, 31964));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32781, 11, 31987));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32797, 11, 31966));
    waypoints.insert(waypoints.begin() + waypoints.size(), new RopeWaypoint(32792, 11, 31960, Direction::CENTER));

    //+1 FROM BOTTOM
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32784, 10, 31954));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32785, 10, 31940));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32780, 10, 31926));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32793, 10, 31941));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32796, 10, 31927));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32807, 10, 31927));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32808, 10, 31943));
    waypoints.insert(waypoints.begin() + waypoints.size(), new RopeWaypoint(32807, 10, 31939, Direction::CENTER));

    //+2 FROM BOTTOM
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32809, 9, 31927));
    waypoints.insert(waypoints.begin() + waypoints.size(), new UseWaypoint(32776, 9, 31932, Direction::CENTER));

    //-1 FROM MAIN
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32758, 8, 31930));
    waypoints.insert(waypoints.begin() + waypoints.size(), new WalkWaypoint(32776, 8, 31949, Direction::EAST));

    //+2 FROM BOTTOM
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32783, 9, 31943));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32796, 9, 31939));
    waypoints.insert(waypoints.begin() + waypoints.size(), new NodeWaypoint(32801, 9, 31944));
    waypoints.insert(waypoints.begin() + waypoints.size(), new WalkWaypoint(32792, 9, 31951, Direction::EAST));

    //+1 FROM BOTTOM
    waypoints.insert(waypoints.begin() + waypoints.size(), new WalkWaypoint(32792, 10, 31959, Direction::SOUTH));
}

void delete_waypoints() {
    for (int i = 0; i < waypoints.size(); i++) {
        delete waypoints[i];
    }
}

int current = 0;
int previous = 0;

void cavebot(Tracking* tracking) {
    if ((int) waypoints.size() < (current - 1)) {
        return;
    }

    //if (waypoints[previous]->getType() == WALK || waypoints[previous]->getType() == USE || waypoints[previous]->getType() == ROPE || waypoints[previous]->getType() == SHOVEL)
    if (tracking->position->z != waypoints[current]->getZ()) {
        if (--current < 0) {
            current = waypoints.size() - 1;
         }
    }

    if (handle(waypoints[current], tracking)) {
        previous = current;

        if (++current == waypoints.size()) {
            current = 0;
        }
    }
}
