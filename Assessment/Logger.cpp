#include "Logger.h"
#include <iostream>
#include <ctime>

Logger::Logger(const std::string& logFilePath)
{
	logFile.open(logFilePath.c_str());

	SDL_LogSetOutputFunction(&Logger::LogCB, (void*)this);
}

Logger::~Logger()
{
	logFile.close();
}

void Logger::LoggingFunction(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	char timeStr[32];
	getTime(timeStr, 32);
	
	std::cout << timeStr << " " << message << std::endl;

	logFile << timeStr << " " << message << std::endl;
}

void Logger::LogCB(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	Logger* ptr = (Logger*)userdata;
	
	if (ptr)
		ptr->LoggingFunction(userdata, category, priority, message);
}

void Logger::getTime(char* buffer, int buffersize)
{
	time_t currentTime = std::time(0);

	struct tm info;
	localtime_s(&info, &currentTime);

	strftime(buffer, buffersize, "%d/%m/%y %T", &info);
}
