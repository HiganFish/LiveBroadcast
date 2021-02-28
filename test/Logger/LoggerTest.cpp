//
// Created by rjd67 on 2020/11/25.
//
#include "utils/Logger.h"

int main()
{
	Logger::SetLogLevel(Logger::DEBUG);

	LOG_DEBUG << "123";
	LOG_INFO << "123";
	LOG_WARN << "123";
	LOG_ERROR << "123";

	Logger::SetLogLevel(Logger::WARN);

	LOG_DEBUG << "123";
	LOG_INFO << "123";
	LOG_WARN << "123";
	LOG_ERROR << "123";

	LOG_FATAL << "123";
}