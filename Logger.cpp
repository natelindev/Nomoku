//
//  Logger.cpp
//  Nomoku
//
//

#include "Logger.h"
#include <ctime>
#include <chrono>
#include <iostream>
#include <cstdarg>
#include <fstream>
#ifndef ENABLE_LOGGER
#define ENABLE_LOGGER 1
#endif

std::string Levels[4] = { "DBUG", "INFO", "WARN", "ERRO" };

void logEvent(int level, std::string log_info, ...){
	if (ENABLE_LOGGER)
	{
		std::ofstream out("Nomoku_log.txt");
		std::time_t now = std::time(nullptr);
		struct tm *nt = std::localtime(&now);
		out << '[' << Levels[level] << ']' \
			<< nt->tm_year + 1900 << '-' << nt->tm_mon + 1 << '-' << nt->tm_mday << ' '
			<< nt->tm_hour << ':' << nt->tm_min << ':' << nt->tm_sec << ' ';
		va_list args;
		va_start(args, log_info);
		vprintf(log_info.c_str(), args);
		va_end(args);
		out << '\n';
		out.close();
	}
}
