#include "essentials.h"

#include <gdiplus.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char buffer[128];
    int written = GetWindowTextA(hwnd, buffer, 128);
    if (written && strstr(buffer, "Windowed") != NULL) {
        *(HWND*)lParam = hwnd;
        return FALSE;
    }
    return TRUE;
}

static HWND hWnd = NULL;

struct Init {
    Init() {
        EnumWindows(EnumWindowsProc, (LPARAM)&hWnd);
    }
};

static Init initInstance;

bool capture_client(BitmapCapture *bmpCapture) {
    ZeroMemory(bmpCapture, sizeof(BitmapCapture));

    HDC hdcScreen = GetWindowDC(hWnd);
    HDC hdcCapture = CreateCompatibleDC(hdcScreen);

    int width = 1920;
    int height = 1080;

    LPBYTE lpCapture;
    BITMAPINFO bmiCapture = { {sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, 0, 0, 0, 0, 0,} };

    bmpCapture->hbm = CreateDIBSection(hdcScreen, &bmiCapture, DIB_RGB_COLORS, (LPVOID*)&lpCapture, NULL, 0);

    bool bResult = false;

    if (bmpCapture->hbm) {
        SelectObject(hdcCapture, bmpCapture->hbm);
        BitBlt(hdcCapture, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
        bmpCapture->pixels = (LPDWORD)lpCapture;
        bmpCapture->width = width;

        bResult = true;
    }

    DeleteDC(hdcCapture);
    ReleaseDC(hWnd, hdcScreen);
    return bResult;
}

bool capture_image(const WCHAR* path, BitmapCapture* bmpCapture) {
    ZeroMemory(bmpCapture, sizeof(BitmapCapture));

    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(path, false);

    if (bitmap) {
        bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &bmpCapture->hbm);
        delete bitmap;
    }

    BITMAP bm;

    GetObject(bmpCapture->hbm, sizeof(BITMAP), &bm);

    if (bmpCapture->hbm) {
        bmpCapture->pixels = (LPDWORD)bm.bmBits;
        bmpCapture->width = bm.bmWidth;

        return true;
    }

    return false;
}

int get_rgb(BitmapCapture* grab, int x, int y, int add) {
    return ARGB_TO_COLORREF(BitmapPixel(grab, x + add, y + add));
}

void click(int x, int y, bool right) {
    INPUT inputs[2] = { 0 };

    SetCursorPos(x, y);

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = !right ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = !right ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;

    SendInput(2, inputs, sizeof(INPUT));
}

void key_press(int key) {
    INPUT input;

    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = key;
    input.ki.dwFlags = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void key_release(int key) {
    INPUT input;

    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}
