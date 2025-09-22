#include "waypoint.h"

Type Waypoint::getType() {
    return Type::STAND;
}

int Waypoint::getX() {
    return x_;
}

int Waypoint::getZ() {
    return z_;
}

int Waypoint::getY() {
    return y_;
}

Type StandWaypoint::getType() {
    return Type::STAND;
}

StandWaypoint::StandWaypoint(int x, int z, int y) {
    x_ = x;
    z_ = z;
    y_ = y;
}

Type NodeWaypoint::getType() {
    return Type::NODE;
}

NodeWaypoint::NodeWaypoint(int x, int z, int y) {
    x_ = x;
    z_ = z;
    y_ = y;
}

Type WalkWaypoint::getType() {
    return Type::WALK;
}

Direction WalkWaypoint::getDirection() {
    return direction_;
}

WalkWaypoint::WalkWaypoint(int x, int z, int y, Direction direction) {
    x_ = x;
    z_ = z;
    y_ = y;
    direction_ = direction;
}

Type UseWaypoint::getType() {
    return Type::USE;
}

Direction UseWaypoint::getDirection() {
    return direction_;
}

UseWaypoint::UseWaypoint(int x, int z, int y, Direction direction) {
    x_ = x;
    z_ = z;
    y_ = y;
    direction_ = direction;
}

Type RopeWaypoint::getType() {
    return Type::ROPE;
}

Direction RopeWaypoint::getDirection() {
    return direction_;
}

RopeWaypoint::RopeWaypoint(int x, int z, int y, Direction direction) {
    x_ = x;
    z_ = z;
    y_ = y;
    direction_ = direction;
}

Type ShovelWaypoint::getType() {
    return Type::SHOVEL;
}

Direction ShovelWaypoint::getDirection() {
    return direction_;
}

ShovelWaypoint::ShovelWaypoint(int x, int z, int y, Direction direction) {
    x_ = x;
    z_ = z;
    y_ = y;
    direction_ = direction;
}
