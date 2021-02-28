//
// Created by rjd67 on 2020/11/24.
//

#ifndef LIVEBROADCASTSERVER_TIMESTAMP_H
#define LIVEBROADCASTSERVER_TIMESTAMP_H

#include <ctime>
#include <cstdint>

class Timestamp
{
public:

	const static int64_t MsPerSecond = 1000;
	const static int64_t UsPerSecond = 1000 * 1000;
	const static int64_t UsPerMs = 1000;

	Timestamp();
	Timestamp(int64_t us_since_epoch);
	~Timestamp();

	static Timestamp Now();

	int64_t GetUsSinceEpoch() const;

private:
	int64_t us_since_epoch_;
};

#endif //LIVEBROADCASTSERVER_TIMESTAMP_H
