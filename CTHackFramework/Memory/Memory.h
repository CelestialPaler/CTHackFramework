// CTHackFramework 														      
//	A framework for general game hacking 									      
// Copyright © 2019 Celestial Tech All Rights Reserved

#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <iomanip> 
#include <sstream>
#include <string>
#include <vector>

#include "Logger.h"
#include "..//Util/StringManipulation.h"

typedef uint64_t QWORD;

/* AoB搜索模式 */
#define BRUTEFORCE 0x1 // 暴力模式，强行覆盖页面原有的保护
#define PAGEBASED 0x2 // 基于页面模式，以页面为单位进行扫描

/* 内存类 */
/// 功能：负责目标内存的读取与写入，负责内存中特征的扫描。
class Memory
{
public:
	// 从目标进程的内存中读取一个数据
	/// 参数：_hProcessHandle：目标进程句柄
	///           _Address：目标内存地址
	/// 返回值：成功-读取的数据   失败-0
	template <class T>
	static T ReadData(const HANDLE _hProcessHandle, const QWORD _qwAddress)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		SIZE_T bytesRead = 0;
		T value = 0;

		// 读取内存
		ReadProcessMemory(_hProcessHandle, (LPVOID)(_qwAddress), &value, sizeof(T), &bytesRead);

		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::FATAL;
			msg.szFrom = __func__;
			msg.szMsg = "Read memory error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
			return 0;
		}
		else
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::DEBUG;
			msg.szFrom = __func__;
			std::stringstream ss;
			ss << "Read " << std::dec << bytesRead << " Bytes memory at 0x" << std::hex << std::uppercase << _qwAddress << " " << std::dec;
			msg.szMsg = ss.str();
			msg.szResult = std::to_string(value);
			logger->Write(msg);
		}
		return value;
	}

	// 向目标进程的内存中写入一个数据
	/// 参数：_hProcessHandle：目标进程句柄
	///           _Address：目标内存地址
	///           _newData：新数据
	/// 返回值：成功-true 失败-false
	template <class T>
	static bool WriteData(const HANDLE _hProcessHandle, const QWORD _qwAddress, const T _newData)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		SIZE_T bytesWritten = 0;

		// 写入内存
		WriteProcessMemory(_hProcessHandle, (LPVOID)_qwAddress, &_newData, sizeof(T), &bytesWritten);

		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::FATAL;
			msg.szFrom = __func__;
			msg.szMsg = "Write memory error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
			return false;
		}
		else
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::DEBUG;
			msg.szFrom = __func__;
			std::stringstream ss;
			ss << "Write " << std::dec << bytesWritten << " Bytes memory at 0x" << std::hex << std::uppercase << _qwAddress << " " << std::dec;
			msg.szMsg = ss.str();
			msg.szResult = _newData;
			logger->Write(msg);
			return true;
		}
	}

	// 从目标进程的内存中读取连续的字节
	/// 参数：_hProcessHandle：目标进程句柄
	///           _Address：目标内存地址
	/// 返回值：读取的字节数组
	static BYTE* ReadBytes(const HANDLE _hProcessHandle, const QWORD _qwAddress, const unsigned int _count)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		SIZE_T bytesRead = 0;
		BYTE* tempData;

		// 读取连续的字节
		ReadProcessMemory(_hProcessHandle, (LPVOID)(_qwAddress), tempData, sizeof(BYTE) * _count, &bytesRead);

		// 处理错误信息
		int err = GetLastError();
		if (err != 0)
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::FATAL;
			msg.szFrom = __func__;
			msg.szMsg = "Read memory error!";
			msg.szCode = std::to_string(err);
			logger->Write(msg);
		}
		else
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::DEBUG;
			msg.szFrom = __func__;
			std::stringstream ss;
			ss << "Read " << std::dec << bytesRead << " Bytes memory at 0x" << std::hex << std::uppercase << _qwAddress << " " << std::dec;
			msg.szMsg = ss.str();
			logger->Write(msg);
		}
		return tempData;
	}

	// 向目标进程的内存中写入连续的字节
	/// 参数：_hProcessHandle：目标进程句柄
	///           _Address：目标内存地址
	///           _newData：新数据
	/// 返回值：成功-true 失败-false
	static bool WriteBytes(const HANDLE _hProcessHandle, const QWORD _qwAddress, const unsigned int _count, const BYTE* _newData)
	{

	}

	/* 特征字节 */
	/// 功能：描述AoB特征的字节
	/// 描述：byte：字节的内容。
	///           enabled：字节是否使能，即是否是通配符。
	struct SignitureByte
	{
		BYTE byte;
		bool enabled;
	};

	// AoB扫描（特征扫描，Pattern扫描）
	/// 参数：_hProcessHandle：目标进程句柄
	///           _szSigniture：需要搜索的特征
	///           _qwAddrBegin：搜索起始地址
	///           _qwAddrEnd：搜索结束地址
	///           _dwMode：搜索模式
	/// 返回值：搜索结果的向量
	static std::vector<DWORD> AoBScan(const HANDLE _hProcessHandle, const std::vector<SignitureByte>& _signiture,
		const DWORD _qwAddrBegin, const DWORD _qwAddrEnd, const DWORD _dwMode)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		// 内存虚拟页面信息存储
		MEMORY_BASIC_INFORMATION memBasicInfo = {};
		// 查询结果
		std::vector<DWORD> result;

		// 确定应用程序的地址空间最大范围
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		const DWORD minAppAddr = reinterpret_cast<DWORD>(systemInfo.lpMinimumApplicationAddress); // 最小地址
		const DWORD maxAppAddr = reinterpret_cast<DWORD>(systemInfo.lpMaximumApplicationAddress); // 最大地址
		const DWORD memPageSize = systemInfo.dwPageSize; // 内存页面大小
		// 防止扫描地址越界
		DWORD addrEnd = _qwAddrEnd > maxAppAddr ? maxAppAddr : _qwAddrEnd;
		DWORD addrBegin = _qwAddrBegin > minAppAddr ? _qwAddrBegin : minAppAddr;
		if (addrBegin > addrEnd) return std::vector<DWORD>();

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::DEBUG;
			msg.szFrom = __func__;
			std::stringstream ss;
			ss << "System Info";
			ss << " | MinAddr: " << std::hex << std::uppercase << systemInfo.lpMinimumApplicationAddress;
			ss << " | MaxAddr: " << std::hex << std::uppercase << systemInfo.lpMaximumApplicationAddress;
			ss << " | MemPageSize: " << std::dec << systemInfo.dwPageSize << " Bytes";
			ss << " | ProcessorArch: " << std::dec << systemInfo.wProcessorArchitecture;
			msg.szMsg = ss.str();

			logger->Write(msg);
		}
		// 扫描指针
		DWORD addrCurrent = addrBegin;

		// 遍历整个扫描区间
		while (addrCurrent < _qwAddrEnd)
		{
			VirtualQueryEx(_hProcessHandle, reinterpret_cast<LPVOID>(addrCurrent), &memBasicInfo, sizeof(MEMORY_BASIC_INFORMATION));
			// 页面组大小为0时搜索结束
			if ((int)memBasicInfo.RegionSize <= 0) break;

			// 如果页面组符合条件
			if ((memBasicInfo.State & MEM_COMMIT) == MEM_COMMIT
				&& (memBasicInfo.Protect & PAGE_GUARD) != PAGE_GUARD
				&& (memBasicInfo.Protect & PAGE_NOACCESS) != PAGE_NOACCESS)
			{
				// 日志
				{
					Util::Logger::LogDisc msg;
					msg.emPriority = Util::Logger::Priority::DEBUG;
					msg.szFrom = __func__;
					msg.szMsg = "AoB scaning...";
					std::stringstream ss;
					ss << "Addr: 0x" << std::hex << std::uppercase << memBasicInfo.BaseAddress <<
						" Size: " << std::setfill(' ') << std::setw(4) << std::dec << (int)(memBasicInfo.RegionSize / memPageSize) << " Pages" <<
						" ProtectFlag: 0x" << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << memBasicInfo.Protect <<
						" Percentage: " << std::setfill(' ') << std::setw(2) << std::dec << (int)((float)(DWORD)memBasicInfo.BaseAddress / (float)(addrEnd - addrBegin) * 100) << "%";
					msg.szTarget = ss.str();
					logger->Write(msg);
				}

				// 如果分割页面
				if ((_dwMode & PAGEBASED) == PAGEBASED)
				{
					// 遍历该组中每一个页面
					for (int i = 0; i < (int)memBasicInfo.RegionSize / memPageSize; i++)
					{
						// 保存原保护
						DWORD oldProtection;
						// 如果开启强力模式
						if ((_dwMode & BRUTEFORCE) == BRUTEFORCE)
						{
							// 设置新保护
							SetVirtualPageProtection(_hProcessHandle, reinterpret_cast<LPVOID>(addrCurrent), memBasicInfo.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtection);
						}

						// 读取内存
						std::vector<BYTE> memoryPage(memPageSize);
						bool flag = ReadProcessMemory(_hProcessHandle, reinterpret_cast<LPVOID>(addrCurrent), reinterpret_cast<LPVOID>(memoryPage.data()), memPageSize, 0);

						// 处理错误信息
						if (flag == false)
						{
							// 日志
							{
								Util::Logger::LogDisc msg;
								msg.emPriority = Util::Logger::Priority::FATAL;
								msg.szFrom = __func__;
								msg.szMsg = "Read memory error!";
								msg.szCode = std::to_string(GetLastError());
								logger->Write(msg);
							}
						}

						// 进行比较
						std::vector<DWORD> offsets = MemCompare(memoryPage, _signiture);

						for (const DWORD offset : offsets)
							result.push_back(addrCurrent + offset);

						// 地址移向下一个页面
						addrCurrent += memPageSize;

						// 如果开启强力模式
						if ((_dwMode & BRUTEFORCE) == BRUTEFORCE)
						{
							// 恢复原保护
							ResetVirtualPageProtection(_hProcessHandle, (LPVOID)addrCurrent, memPageSize, oldProtection);
						}
					}
				}
				// 如果不分割页面
				else
				{
					// 保存原保护
					DWORD oldProtection;
					// 如果开启强力模式
					if ((_dwMode & BRUTEFORCE) == BRUTEFORCE)
					{
						// 设置新保护
						SetVirtualPageProtection(_hProcessHandle, reinterpret_cast<LPVOID>(addrCurrent), memBasicInfo.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtection);
					}

					// 读取内存
					std::vector<BYTE> memoryPage(memBasicInfo.RegionSize);
					bool flag = ReadProcessMemory(_hProcessHandle, reinterpret_cast<LPVOID>(addrCurrent), reinterpret_cast<LPVOID>(memoryPage.data()), memBasicInfo.RegionSize, 0);

					// 处理错误信息
					if (flag == false)
					{
						// 日志
						{
							Util::Logger::LogDisc msg;
							msg.emPriority = Util::Logger::Priority::FATAL;
							msg.szFrom = __func__;
							msg.szMsg = "Read memory error!";
							msg.szCode = std::to_string(GetLastError());
							logger->Write(msg);
						}
					}

					// 进行比较
					std::vector<DWORD> offsets = MemCompare(memoryPage, _signiture);

					for (const DWORD offset : offsets)
						result.push_back(addrCurrent + offset);

					// 地址移向下一个页面
					addrCurrent += memBasicInfo.RegionSize;

					// 如果开启强力模式
					if ((_dwMode & BRUTEFORCE) == BRUTEFORCE)
					{
						// 恢复原保护
						ResetVirtualPageProtection(_hProcessHandle, (LPVOID)addrCurrent, memPageSize, oldProtection);
					}
				}
			}
			// 如果不符合则跳过该页面组
			else
			{
				// 日志
				{
					Util::Logger::LogDisc msg;
					msg.emPriority = Util::Logger::Priority::DEBUG;
					msg.szFrom = __func__;
					msg.szMsg = "AoB scaning...";
					std::stringstream ss;
					ss << "Addr: 0x" << std::hex << std::uppercase << memBasicInfo.BaseAddress <<
						" Size: " << std::setfill(' ') << std::setw(4) << std::dec << (int)(memBasicInfo.RegionSize / memPageSize) << " Pages" <<
						" ProtectFlag: 0x" << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << memBasicInfo.Protect <<
						" Percentage: " << std::setfill(' ') << std::setw(2) << std::dec << (int)((float)(DWORD)memBasicInfo.BaseAddress / (float)(addrEnd - addrBegin) * 100) << "%";
					msg.szTarget = ss.str();
					msg.szResult = "Skipped";
					logger->Write(msg);
				}

				addrCurrent += memBasicInfo.RegionSize;
			}
		}

		// 扫描过程中是否有错误
		int err = GetLastError();
		if (err != 0)
		{
			// 日志
			{
				Util::Logger::LogDisc msg;
				msg.emPriority = Util::Logger::Priority::FATAL;
				msg.szFrom = __func__;
				msg.szMsg = "AoB scan error!";
				msg.szCode = std::to_string(err);
				logger->Write(msg);
			}

			return std::vector<DWORD>();
		}

		// 扫描完毕但是没有结果
		if (result.size() == 0)
		{
			// 日志
			{
				Util::Logger::LogDisc msg;
				msg.emPriority = Util::Logger::Priority::WARNING;
				msg.szFrom = __func__;
				std::stringstream ss;
				ss << "AoB scan finished! Signiture not found!";
				msg.szMsg = ss.str();
				logger->Write(msg);
			}

			return std::vector<DWORD>();
		}

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "AoB scan finished!";
			std::stringstream ss;
			ss << "Signiture found in total: " << std::dec << result.size();
			msg.szResult = ss.str();
			logger->Write(msg);
		}

		// 返回扫描结果
		return result;
	}

public:
	// 将特征打印于Log
	static void SignitureToLog(const std::vector<SignitureByte>& _signiture)
	{
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();
		// 日志
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "AoB Signiture";
		std::stringstream ss;
		for (size_t i = 0; i < _signiture.size(); i++)
		{
			if (_signiture.at(i).enabled)
				ss << std::hex << std::setfill('0') << std::setw(2) << (int)_signiture.at(i).byte << " ";
			else
				ss << "??" << " ";
		}
		msg.szResult = ss.str();
		logger->Write(msg);
	}

	// 将AoB搜索结果打印于Log
	static void AoBResultToLog(const std::vector<DWORD>& _result)
	{
		for (size_t i = 0; i < _result.size(); i++)
			std::cout << std::dec << i + 1 << " 0x" << std::hex << std::uppercase << _result.at(i) << std::endl;
	}

private:
	// 设置目标进程内存虚拟页面保护
	/// 参数：_hProcessHandle：目标进程句柄
	///           _mbi：目标内存页面信息
	///           _newProtect：新的保护
	///           _oldProtect：旧的保护
	/// 返回值：成功-true 失败-false
	static bool SetVirtualPageProtection(const HANDLE& _hProcessHandle, const LPVOID _lpAddress, const SIZE_T _regionSize, const DWORD _newProtect, const PDWORD _oldProtect)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		VirtualProtectEx(_hProcessHandle, _lpAddress, _regionSize, _newProtect, _oldProtect);
		int err = GetLastError();
		if (err != 0)
		{
			// 日志
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Set protection of virtual memory error!";
			std::stringstream ss;
			ss << "Addr: 0x" << std::hex << std::uppercase << _lpAddress;
			msg.szTarget = ss.str();
			msg.szCode = std::to_string(err);
			logger->Write(msg);
			return false;
		}
		return true;
	}

	// 恢复目标进程内存虚拟页面保护
	/// 参数：_hProcessHandle：目标进程句柄
	///           _mbi：目标内存页面信息
	///           _oldProtect：旧的保护
	/// 返回值：成功-true 失败-false
	static bool ResetVirtualPageProtection(const HANDLE _hProcessHandle, const LPVOID _lpAddress, const SIZE_T _regionSize, const DWORD _oldProtect)
	{
		// 重置错误信息
		SetLastError(0);
		// 取得Logger
		Util::Logger* logger = Util::Logger::GetInstance();

		DWORD tempProtect;
		VirtualProtectEx(_hProcessHandle, _lpAddress, _regionSize, _oldProtect, &tempProtect);
		int err = GetLastError();
		if (err != 0)
		{
			// 日志
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::WARNING;
			msg.szFrom = __func__;
			msg.szMsg = "Reset protection of virtual memory error!";
			std::stringstream ss;
			ss << "Addr: 0x" << std::hex << std::uppercase << _lpAddress;
			msg.szTarget = ss.str();
			msg.szCode = std::to_string(err);
			logger->Write(msg);
			return false;
		}
		return true;
	}

	// 字符串转为特征
	/// 将带通配符的字符串转化为一组特征字节
	static std::vector<SignitureByte> StringToSigniture(const std::string _str)
	{
		std::vector<SignitureByte> signiture;

		std::vector<std::string> segments = Util::StringManipulation::SplitString(_str, " ");
		for (const std::string& segment : segments)
		{
			if (segment == "??")
				signiture.emplace_back(SignitureByte{ 0, false });
			else
				signiture.emplace_back(SignitureByte{ static_cast<BYTE>(std::stoi(segment, nullptr, 16)), true });
		}
		return signiture;
	}

	// 内存比较
	/// 将特征_signiture在_memory中进行比较，将获得的偏移地址全部保存在向量中
	static std::vector<DWORD> MemCompare(const std::vector<BYTE>& _memory, const std::vector<SignitureByte>& _signiture)
	{
		return BruteForce(_memory, _signiture);
	}

	// 暴力字符串匹配算法
	static std::vector<DWORD> BruteForce(const std::vector<BYTE>& _memory, const std::vector<SignitureByte>& _signiture)
	{
		std::vector<DWORD> offset;
		for (size_t i = 0; i < _memory.size() - _signiture.size(); i++)
		{
			bool flag = true;
			for (size_t j = 0; j < _signiture.size(); j++)
			{
				if (_signiture.at(j).enabled && _signiture.at(j).byte != _memory.at(i + j))
				{
					flag = false;
					break;
				}
			}
			if (flag)
				offset.push_back(i);
		}
		return offset;
	}

	// Sunday字符串匹配算法
	static std::vector<DWORD> Sunday(const std::vector<BYTE>& _memory, const std::vector<SignitureByte>& _signiture)
	{
		std::vector<DWORD> offset;

		size_t i = 0;
		// 遍历主串
		while (i < _memory.size() - _signiture.size())
		{
			size_t k = 0;
			// 比较主串与子串
			for (size_t j = 0; j < _signiture.size(); j++)
			{
				// 若失配，则看主串失配部分下一个字符在字串中的位置
				if (_signiture.at(j).enabled && _memory.at(i + j) != _signiture.at(j).byte)
				{
					size_t m = _signiture.size();
					for (size_t j = 0; j < _signiture.size(); j++)
					{
						// 若在子串中找到，则计算下次移动距离
						if ((_signiture.at(j).enabled && _memory.at(i + _signiture.size()) == _signiture.at(j).byte) || !_signiture.at(j).enabled)
							// 不是通配符就要相同，是通配符就无所谓，就当是相同即可
						{
							m = _signiture.size() - j;
							break;
						}
					}
					// 进行移动
					i += m;
					break;
				}
				// 若匹配
				else
					k++;

				if (k == _signiture.size())
				{
					offset.push_back(i);
					i += _signiture.size();
				}
			}
		}
		return offset;
	}
};
