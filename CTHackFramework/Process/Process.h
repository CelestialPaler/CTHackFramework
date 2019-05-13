// CTHackFramework 														      
//	A framework for general game hacking 									      
// Copyright © 2019 Celestial Tech All Rights Reserved

#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <TlHelp32.h>
#include <iostream>

#include "../Memory/Memory.h"

/* 进程类 */
/// 功能：负责目标进程信息的读取，所包含模块的信息的读取。
///           负责对该进程内存的相关操作，如读写，特征扫描等。
class Process
{
public:
	// 构造函数
	Process() {}
	// 禁止复制初始化与赋值操作
	Process(const Process&) = delete;
	Process& operator = (const Process&) = delete;
	// 析构函数
	~Process() {}

public:
	// 连接目标进程
	/// 参数：_szProcessName：目标进程的名称
	/// 返回值：成功-true   失败-false
	bool Attach(const std::wstring& _szProcessName);

	// 连接目标进程
	/// 参数：_dwProcessID：目标进程ID
	/// 返回值：成功-true   失败-false
	bool Attach(const DWORD& _dwProcessID);

	// 断开与目标进程的链接
	void Dettach(void);

public:
	// AoB扫描（特征扫描，Pattern扫描）
	/// 参数：_signitureStr：需要搜索的特征字符串
	///           _qwAddrBegin：搜索起始地址
	///           _qwAddrEnd：搜索结束地址
	///           _dwMode：搜索模式
	/// 搜索模式：BRUTEFORCE 暴力模式：强行覆盖页面原保护，然后进行扫描
	///                  PAGEBASED 基于页面模式：一页面为单位进行扫描
	/// 返回值：搜索结果的向量
	std::vector<DWORD> AoBScan(const std::string& _signitureStr, const DWORD& _dwAddrBegin = 0x0, const DWORD & _dwAddrEnd = 0x7FFFFFFF, const DWORD & _dwModeFlag = NULL);

public:
	// 读取一个字节
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	BYTE ReadByte(const DWORD& _dwAddr);
	// 读取一个16位有符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	INT16 ReadInt16(const DWORD& _dwAddr);
	// 读取一个32位有符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	INT32 ReadInt32(const DWORD& _dwAddr);
	// 读取一个64位有符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	INT64 ReadInt64(const DWORD& _dwAddr);
	// 读取一个16位无符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	UINT16 ReadUint16(const DWORD& _dwAddr);
	// 读取一个32位无符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	UINT32 ReadUint32(const DWORD& _dwAddr);
	// 读取一个64位无符号整型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	UINT64 ReadUint64(const DWORD& _dwAddr);
	// 读取一个单精度浮点型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	FLOAT ReadFloat(const DWORD& _dwAddr);
	// 读取一个双精度浮点型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	DOUBLE ReadDouble(const DWORD& _dwAddr);
	// 读取一个布尔型
	/// 参数：_dwAddr：读取的地址
	/// 返回值：成功-读取的数据   失败-0
	BOOL ReadBool(const DWORD& _dwAddr);

	// 写入一个字节
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteByte(const DWORD& _dwAddr, const BYTE& _newData);
	// 写入一个16位有符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteInt16(const DWORD& _dwAddr, const INT16& _newData);
	// 写入一个32位有符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteInt32(const DWORD& _dwAddr, const INT32& _newData);
	// 写入一个64位有符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteInt64(const DWORD& _dwAddr, const INT64& _newData);
	// 写入一个16位无符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteUint16(const DWORD& _dwAddr, const UINT16& _newData);
	// 写入一个32位无符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteUint32(const DWORD& _dwAddr, const UINT32& _newData);
	// 写入一个64位无符号整型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteUint64(const DWORD& _dwAddr, const UINT64& _newData);
	// 写入一个单精度浮点型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteFloat(const DWORD& _dwAddr, const FLOAT& _newData);
	// 写入一个双精度浮点型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteDouble(const DWORD& _dwAddr, const DOUBLE& _newData);
	// 写入一个布尔型
	/// 参数：_dwAddr：写入的地址
	///           _newData：写入的新数据
	/// 返回值：成功-true   失败-false
	bool WriteBool(const DWORD& _dwAddr, const BOOL& _newData);

	std::vector<BYTE> ReadBytes(const DWORD& _dwAddr, const SIZE_T& _size);
	bool WriteBytes(const DWORD& _dwAddr, const std::vector<BYTE>& _newData);

private:
	// 设置目标进程名称
	void SetTargetProcess(const std::wstring& _szProcessName);
	// 获取目标进程的PID
	void GetProcessID(void);
	// 获取目标进程的句柄
	void OpenProcessHandle(void);
	// 关闭目标进程的句柄
	void CloseProcessHandle(void);
	// 获取所有模块的基地址
	void GetModuleBaseAddress(void);

private:
	// 设置特权Token
	BOOL SetPrivilege(const HANDLE _hToken, const LPCTSTR _szPrivilege, const BOOL _enablePrivilege);
	// 获取Debug特权（需要管理员权限）
	BOOL GetDebugPrivileges(void);

public:
	// 模块基地址
	std::map<const std::wstring, const QWORD> moduleBaseAddr;

private:
	std::wstring szProcessName = L"";
	DWORD dwProcessID = 0;
	HANDLE hProcessHandle = INVALID_HANDLE_VALUE;
};

