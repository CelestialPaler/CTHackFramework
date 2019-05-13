/***************************************************************************************************/
/*								        		 		  CTProcess Project 														      */
/*								         A library for External/Internal game hacking 									  */
/*								        		 			  Logger Class     														      */
/*                                        Copyright © 2015-2019 Celestial Tech                                              */
/***************************************************************************************************/
#pragma once

// Header file
#include <string>
#include <mutex>
#include <fstream>
#include <iostream>
#include <chrono>

#include "ConsoleColor.h"

/***************************************************************************************************/
// Namespace : Util
/// Basic utilities.
namespace Util
{
	/***************************************************************************************************/
	// Class : Logger
	/// Implemented in singleton patern with multithread security.
	/// Do the nasty logging thingy ... 
	class Logger
	{
	public:
		/***************************************************************************************************/
		// Enum : Logger priority
		enum Priority
		{
			DEBUG,
			INFO,
			WARNING,
			FATAL
		};

		/***************************************************************************************************/
		// struct : Logger Discriptor
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
		// Get the pointer of the instance
		/// Singleton
		static Logger* GetInstance(void);

	public:
		// Start the logger
		/// - _priorityThreshold : Logger only log the message with priority above the threshold.
		/// - _logFile : The path to logfile. ("" for logging to std::cout)
		static void Start(Priority _priorityThreshold = WARNING, const std::string& _logFile = "");
		// Stop the logger
		static void Stop(void);
		// Write log
		static void Write(LogDisc _logDisc);

	private:
		Logger(void) {}
		Logger(const Logger& _logger) = delete;
		Logger& operator = (const Logger& _logger) = delete;

	private:
		// Get the current system time.
		const std::string GetCurrentSystemTime() const;

	private:
		// is the logger activated 
		bool activated;
		std::ofstream logFileStream;
		Priority priorityThreshold;

	private:
		const std::string priorityNames[4] =
		{
			"[DEBUG]  ",
			"[INFO]   ",
			"[WARNING]",
			"[FATAL]  "
		};
	};

	std::wstring StringToWstring(std::string _szString);

	std::string WstringToString(std::wstring _szWstring);
}
