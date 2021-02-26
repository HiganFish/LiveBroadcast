//
// Created by rjd67 on 2021/2/26.
//

#include <cstdint>
#include <syscall.h>
#include <unistd.h>
#include "CurrentThread.h"

namespace CurrentThread
{
__thread int t_cached_tid = 0;

void CacheTid();

void CacheTid()
{
	t_cached_tid = static_cast<int>(::syscall(SYS_gettid));
}
}