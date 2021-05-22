//
// Created by rjd67 on 2020/11/25.
//

#include <iomanip>
#include <cassert>
#include <iostream>
#include "thread/CurrentThread.h"
#include "Logger.h"

void DefaultOutput(const char* data, size_t length)
{
	fwrite(data, 1, length, stdout);
}

void DefaultFlush()
{
	fflush(stdout);
}

void (*g_output)(const char* data, size_t length) = DefaultOutput;
void (*g_flush)() = DefaultFlush;

Logger::LogLevel g_loglevel = Logger::INFO;

void Logger::SetLogLevel(LogLevel level)
{
	g_loglevel = level;
}

Logger::Logger(Logger::LogLevel level, const char* file_name, int line):
	impl_(level, file_name, line)
{

}
Logger::~Logger()
{
	impl_.Finish();

	const LoggerBuffer& buffer = GetStream().GetBuffer();

	if (g_output)
	{
		g_output(buffer.ReadBegin(), buffer.GetLength());
	}
	if (impl_.level_ == Logger::FATAL)
	{
		if (g_flush)
		{
			g_flush();
		}
		abort();
	}
}

LoggerStream& Logger::GetStream()
{
	return impl_.stream_;
}

const char* LogLevelStr[] = {
		"DEBUG ",
		"INFO  ",
		"WARN  ",
		"ERROR ",
		"FATAL "
};

Logger::Impl::Impl(Logger::LogLevel level, const char* file_name, int line):
	stream_(),
	time_(Timestamp::Now()),
	level_(level),
	file_name_(file_name),
	line_(line)
{
	FormatTime();

	stream_ << CurrentThread::GetTid() << ' ';
	stream_ << std::string_view(LogLevelStr[static_cast<int>(level)], 6);
}

__thread int last_second;
__thread char time_str[64];

void Logger::Impl::FormatTime()
{
	int64_t us_since_epoch = time_.GetUsSinceEpoch();
	time_t second = static_cast<time_t>(us_since_epoch / Timestamp::UsPerSecond);
	int us = static_cast<int>(us_since_epoch % Timestamp::UsPerSecond);

	if (second != last_second)
	{
		last_second = second;

		struct tm tm;
		gmtime_r(&second, &tm);

		int len = snprintf(time_str, sizeof time_str, "%4d%02d%02d %02d:%02d:%02d",
				tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
				tm.tm_hour, tm.tm_min, tm.tm_sec);
		assert(len == 17);
	}

	int us_len = snprintf(time_str + 17, sizeof time_str - 17, ".%06d ", us);
	assert(us_len == 8);

	stream_ << std::string_view(time_str, 17 + 8);
}

void Logger::Impl::Finish()
{
	stream_ << " - " << file_name_ << ':' << line_ << '\n';
}
