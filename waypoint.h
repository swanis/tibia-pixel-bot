#ifndef WAYPOINT_H
#define WAYPOINT_H

enum Type { STAND, NODE, WALK, USE, ROPE, SHOVEL };
enum Direction { EAST, NORTH_EAST, NORTH, NORTH_WEST, WEST, SOUTH_WEST, SOUTH, SOUTH_EAST, CENTER };

class Waypoint {
protected:
    int x_;
    int z_;
    int y_;
public:
    virtual Type getType();
    int getX();
    int getZ();
    int getY();
};

class StandWaypoint : public Waypoint {
public:
    Type getType();

    StandWaypoint(int x, int z, int y);
};

class NodeWaypoint : public Waypoint {
public:
    Type getType();

    NodeWaypoint(int x, int z, int y);
};

class WalkWaypoint : public Waypoint {
private:
    Direction direction_;
public:
    Type getType();
    Direction getDirection();

    WalkWaypoint(int x, int z, int y, Direction direction);
};

class UseWaypoint : public Waypoint {
private:
    Direction direction_;
public:
    Type getType();
    Direction getDirection();

    UseWaypoint(int x, int z, int y, Direction direction);
};

class RopeWaypoint : public Waypoint {
private:
    Direction direction_;
public:
    Type getType();
    Direction getDirection();

    RopeWaypoint(int x, int z, int y, Direction direction);
};

class ShovelWaypoint : public Waypoint {
private:
    Direction direction_;
public:
    Type getType();
    Direction getDirection();

    ShovelWaypoint(int x, int z, int y, Direction direction);
};

#endif