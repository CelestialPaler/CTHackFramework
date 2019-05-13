#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <mutex>

#include "GameStruct.h"

// 当前开发版本
extern const std::wstring devVersion;

// 目标进程ID
extern DWORD targetPID;
// 目标进程名称
extern const std::wstring targetProcName;
// 目标窗口名称
extern const std::wstring targetWndName;

// 目标窗口句柄
extern HWND hTargetWnd;
// Overlay窗口句柄
extern HWND hOverlayWnd;
// 目标窗口Rect
extern RECT targetRect;
// 目标窗口宽度
extern unsigned int targetWndWidth;
// 目标窗口长度
extern unsigned int targetWndHeight;