//
// Created by rjd67 on 2020/11/24.
//

#include <sys/time.h>
#include "Timestamp.h"

Timestamp::Timestamp()
{

}
Timestamp::Timestamp(int64_t us_since_epoch):
	us_since_epoch_(us_since_epoch)
{

}
Timestamp::~Timestamp()
{

}

int64_t Timestamp::GetUsSinceEpoch() const
{
	return us_since_epoch_;
}

Timestamp Timestamp::Now()
{
	struct timeval time;
	gettimeofday(&time, nullptr);
	return Timestamp(Timestamp::UsPerSecond * time.tv_sec + time.tv_usec);
}
