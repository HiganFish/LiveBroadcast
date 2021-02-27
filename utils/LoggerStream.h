//
// Created by rjd67 on 2021/2/27.
//

#ifndef LIVEBROADCASTSERVER_LOGGERSTREAM_H
#define LIVEBROADCASTSERVER_LOGGERSTREAM_H

#include <string>

/**
 * 用于日志的简单缓冲区
 */
class LoggerBuffer
{
public:

	const static size_t DEFAULT_LOGGER_BUFFER_SIZE = 512;

	LoggerBuffer();

	void Append(const char* data, size_t length);

	const char* ReadBegin() const;
	size_t GetLength() const;

private:

	size_t length_;
	char buffer_[DEFAULT_LOGGER_BUFFER_SIZE];

	char* WriteBegin();

	void AddWriteIdx(size_t length);
};

/**
 * 流式日式类
 */
class LoggerStream
{
public:

	LoggerStream();
	~LoggerStream();

	LoggerStream& operator<<(const std::string& str);
	LoggerStream& operator<<(const char* str);
	LoggerStream& operator<<(char c);
	LoggerStream& operator<<(const std::string_view& view);
	LoggerStream& operator<<(int);

	LoggerBuffer& GetBuffer();

private:
	LoggerBuffer buffer_;
};
#endif //LIVEBROADCASTSERVER_LOGGERSTREAM_H
