#include "GameData.h"

// 当前开发版本
const std::wstring devVersion;

// 目标进程ID
DWORD targetPID;
// 目标进程名称
std::wstring targetProcName;
// 目标窗口名称
std::wstring targetWndName;

// 目标窗口句柄
HWND hTargetWnd;
// 目标窗口Rect
RECT targetRect;
// 目标窗口宽度
unsigned int targetWndWidth;
// 目标窗口长度
unsigned int targetWndHeight;