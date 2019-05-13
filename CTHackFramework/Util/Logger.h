// CTHackFramework 														      
//	A framework for general game hacking 									      
// Copyright © 2019 Celestial Tech All Rights Reserved

#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <iostream>
#include <chrono>

#include "ConsoleColor.h"
#include "StringManipulation.h"

namespace Util
{
	/* 日志类 */
	/// 功能：负责日志记录，将日志按照格式输出。默认输出至stdout，可重定向至日志文件等。
	/// 描述：使用单例模式，确保只有一个日志实例。
	class Logger
	{
	public:
		/*  优先级 */
		/// 功能：设置每条日志记录的优先级，便于查找重要信息。优先级依次增高。
		/// 描述：DEBUG为调试信息，用于表示测试的信息。
		///           INFO为提示信息，用于表示运行状态与进程。
		///           WARNING为警告信息，用于表示可能引起故障的信息。
		///           ERR为错误信息，用于表示程序发生错误，未正常执行。
		///           FATAL为紧急信息，用于表示致命错误，程序可能崩溃。
		enum Priority
		{
			DEBUG,
			INFO,
			WARNING,
			ERR,
			FATAL
		};

		/* 日志记录描述结构体 */
		/// 功能：格式化描述单条记录
		/// 描述：szFrom：表示进行日志登录的发起方，即需要写日志的函数。一般设置为__func__。
		///           szMsg：表示该日志记录的内容，即日志记录的主要内容。
		///           szCode：表示错误代码或返回值。
		///           szTarget：表示该条记录所指的对象，即发起方想要描述的对象。
		///           szResult：表示该条记录所得到的结果。
		///           emPriority：表示该条记录的优先级。   
		struct LogDisc
		{
			std::string szFrom;
			std::string szMsg;
			std::string szCode;
			std::string szTarget;
			std::string szResult;
			Priority emPriority;
		};

	public:
		// 获取实例
		/// 返回值：指向该实例的指针
		static Logger* GetInstance(void);

	public:
		// 开始将记录写入日志
		/// 参数：_priorityThreshold：进行记录登记的最低优先级
		///           _logFile：日志文件目录，缺省则默认为stdout
		static void Start(const Priority _priorityThreshold = WARNING, const std::string& _logFile = "");
		// 停止将记录写入日志
		static void Stop(void);
		// 写入日志
		/// 参数：_logDisc：日志记录描述结构体
		static void Write(const LogDisc& _logDisc);

	private:
		Logger(void) {}
		Logger(const Logger& _logger) = delete;
		Logger& operator = (const Logger& _logger) = delete;

	private:
		// 按照格式获取当前系统时间
		/// 返回值：当前系统时间字符串
		const std::string GetCurrentSystemTime() const;

	private:
		bool activated;
		std::ofstream logFileStream;
		Priority priorityThreshold;

	private:
		const std::string priorityNames[5] =
		{
			"[DEBUG]  ",
			"[INFO]   ",
			"[WARNING]",
			"[ERROR]",
			"[FATAL]  "
		};
	};
}
