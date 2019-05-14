#pragma once

#include <string>
#include <vector>

#include "../Process/Process.h"
#include "../Game/GameStruct.h"
#include "../Game/GameData.h"

class Manager
{
public:
	// 构造函数
	Manager();
	// 禁止复制初始化与赋值操作
	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;
	// 析构函数
	~Manager();

public:
	void HandleKeyBoardInput();
	void HandleMouseInput();
	void UpdateFromConfigFile();

private:
};