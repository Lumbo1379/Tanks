#pragma once
#include <SDL.h>
#include <fstream>

class Logger
{
	public:
		Logger(const std::string& logFilePath);
		~Logger();
		void LoggingFunction(void* userdata, int category, SDL_LogPriority priority, const char* message);
		static void LogCB(void* userdata, int category, SDL_LogPriority priority, const char* message);
	
	private:
		void getTime(char* buffer, int buffersize);
	
		std::ofstream logFile;
};
