//
// Created by rjd67 on 2020/11/25.
//

#ifndef LIVEBROADCASTSERVER_LOGGER_H
#define LIVEBROADCASTSERVER_LOGGER_H

#include <cstdio>
#include <cstring>
#include "utils/Timestamp.h"
#include "utils/LoggerStream.h"

class Logger
{
public:

	enum LogLevel
	{
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	Logger(LogLevel level, const char* file_name, int line);
	~Logger();

	LoggerStream& GetStream();

	static LogLevel GetLogLevel();
	static void SetLogLevel(LogLevel level);

private:

	class Impl
	{
	public:

		Impl(LogLevel level, const char* file_name, int line);

		/**
		 * 格式化时间到stream
		 */
		void FormatTime();

		/**
		 * 添加尾部的文件信息和换行
		 */
		void Finish();

		LoggerStream stream_;

		Timestamp time_;

		LogLevel level_;

		const char* file_name_;

		int line_;

	};

	Impl impl_;
};

extern Logger::LogLevel g_loglevel;
inline Logger::LogLevel Logger::GetLogLevel()
{
	return g_loglevel;
}


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG if (Logger::GetLogLevel() <= Logger::DEBUG)\
Logger(Logger::DEBUG, __FILENAME__, __LINE__).GetStream()
#define LOG_INFO if (Logger::GetLogLevel() <= Logger::INFO)\
Logger(Logger::INFO, __FILENAME__, __LINE__).GetStream()
#define LOG_WARN if (Logger::GetLogLevel() <= Logger::WARN)\
Logger(Logger::WARN, __FILENAME__, __LINE__).GetStream()
#define LOG_ERROR if (Logger::GetLogLevel() <= Logger::ERROR)\
Logger(Logger::ERROR, __FILENAME__, __LINE__).GetStream()
#define LOG_FATAL if (Logger::GetLogLevel() <= Logger::FATAL)\
Logger(Logger::FATAL, __FILENAME__, __LINE__).GetStream()

#endif //LIVEBROADCASTSERVER_LOGGER_H
