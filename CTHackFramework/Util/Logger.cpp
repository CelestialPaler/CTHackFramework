/***************************************************************************************************/
/*								        		 		  Hammann Project  														      */
/*								        		 			  Logger Class     														      */
/*                                                    www.tianshicangxie.com                                                       */
/*                                         Copyright © 2015-2019 Celestial Tech                                              */
/***************************************************************************************************/
#include "Logger.h"

std::mutex mutexLogger;
static Util::Logger* instancePointer;

Util::Logger* Util::Logger::GetInstance(void)
{
	mutexLogger.lock();
	if (instancePointer == nullptr)
		instancePointer = new Logger();
	mutexLogger.unlock();
	return instancePointer;
}

void Util::Logger::Start(Priority _priorityThreshold, const std::string& _logFile)
{
	instancePointer->activated = true;
	instancePointer->priorityThreshold = _priorityThreshold;
	if (_logFile != "")
		instancePointer->logFileStream.open(_logFile.c_str());
}

void Util::Logger::Stop(void)
{
	instancePointer->activated = false;
	if (instancePointer->logFileStream.is_open())
		instancePointer->logFileStream.close();
}

void Util::Logger::Write(const LogDisc& _logDisc)
{
	if (instancePointer->activated && _logDisc.emPriority >= instancePointer->priorityThreshold)
	{
		std::ostream& stream = instancePointer->logFileStream.is_open() ? instancePointer->logFileStream : std::cout;
		stream << instancePointer->GetCurrentSystemTime() << "   ";
		switch (_logDisc.emPriority)
		{
		case Util::Logger::DEBUG:
			stream << white;
			break;
		case Util::Logger::INFO:
			stream << green;
			break;
		case Util::Logger::WARNING:
			stream << yellow;
			break;
		case Util::Logger::ERR:
			stream << red;
			break;
		case Util::Logger::FATAL:
			stream << purple;
			break;
		default:
			break;
		}
		stream << instancePointer->priorityNames[_logDisc.emPriority];
		if (_logDisc.szFrom != "")
			stream << " | From: " << _logDisc.szFrom;
		if (_logDisc.szMsg != "")
			stream << " | Info: " << _logDisc.szMsg;
		if (_logDisc.szTarget != "")
			stream << " | Target: " << _logDisc.szTarget;
		if (_logDisc.szCode != "")
			stream << " | Code: " << _logDisc.szCode;
		if (_logDisc.szResult != "")
			stream << " | Result: " << _logDisc.szResult;
		stream << white;
		stream << std::endl;
	}
}

const std::string Util::Logger::GetCurrentSystemTime() const
{
	__time64_t sysTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm pTime;
	localtime_s(&pTime, &sysTime);
	char timeInString[60] = { 0 };
	sprintf_s(timeInString, "%d-%02d-%02d %02d:%02d:%02d",
		(int)pTime.tm_year + 1900,
		(int)pTime.tm_mon + 1,
		(int)pTime.tm_mday,
		(int)pTime.tm_hour,
		(int)pTime.tm_min,
		(int)pTime.tm_sec);
	return std::string(timeInString);
}

