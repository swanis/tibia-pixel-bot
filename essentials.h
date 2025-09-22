#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <windows.h>
#include <chrono>

#define ARGB_TO_COLORREF(a) (COLORREF)(((a) & 0xFF00FF00) | (((a) & 0xFF0000) >> 16) | (((a) & 0xFF) << 16)) // ARGB to ABGR
#define BitmapPixel(b, x, y) ((b)->pixels[(y) * (b)->width + (x)]) // pixel is ARGB

using namespace std;

struct BitmapCapture {
    HBITMAP hbm;
    LPDWORD pixels;
    INT width;
    INT height;
};

struct Position {
    unsigned short x;
    unsigned short z;
    unsigned short y;
};

struct Tracking {
    Position* position;
    chrono::steady_clock::time_point last_move;
};

struct Point {
    unsigned short x;
    unsigned short y;
};

bool capture_client(BitmapCapture *bmpCapture);
bool capture_image(const WCHAR* path, BitmapCapture* bmpCapture);
int get_rgb(BitmapCapture* grab, int x, int y, int add);
void click(int x, int y, bool right);
void key_press(int key);
void key_release(int key);

#endif
