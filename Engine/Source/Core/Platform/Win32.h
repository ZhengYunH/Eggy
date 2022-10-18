#pragma once

#if defined(_WIN32)
#define NOMINMAX // suppress windef.h min\max
#include <windows.h>

typedef HWND WINDOW_HANDLE;

#endif