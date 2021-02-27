//
// Created by rjd67 on 2021/2/27.
//

#include <cstring>
#include <algorithm>
#include "LoggerStream.h"

LoggerBuffer::LoggerBuffer():
	length_(0)
{

}

void LoggerBuffer::Append(const char* data, size_t length)
{
	std::copy(data, data + length, WriteBegin());
	AddWriteIdx(length);
}

const char* LoggerBuffer::ReadBegin() const
{
	return buffer_;
}
size_t LoggerBuffer::GetLength() const
{
	return length_;
}

char* LoggerBuffer::WriteBegin()
{
	return &buffer_[length_];
}

void LoggerBuffer::AddWriteIdx(size_t length)
{
	length_ += length;
}

LoggerStream::LoggerStream()
{

}
LoggerStream::~LoggerStream()
{

}
LoggerStream& LoggerStream::operator<<(const std::string& str)
{
	buffer_.Append(str.data(), str.length());
	return *this;
}

LoggerStream& LoggerStream::operator<<(const char* str)
{
	buffer_.Append(str, strlen(str));
	return *this;
}

LoggerStream& LoggerStream::operator<<(char c)
{
	buffer_.Append(&c, 1);
	return *this;
}

LoggerStream& LoggerStream::operator<<(const std::string_view& view)
{
	buffer_.Append(view.data(), view.length());
	return *this;
}

LoggerStream& LoggerStream::operator<<(int val)
{
	return *this << std::to_string(val);
}
LoggerBuffer& LoggerStream::GetBuffer()
{
	return buffer_;
}
