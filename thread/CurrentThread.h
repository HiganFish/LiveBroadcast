//
// Created by rjd67 on 2021/2/26.
//

#ifndef LIVEBROADCASTSERVER_CURRENTTHREAD_H
#define LIVEBROADCASTSERVER_CURRENTTHREAD_H

namespace CurrentThread
{
extern __thread int t_cached_tid;

void CacheTid();

inline int GetTid()
{
	if (__builtin_expect(t_cached_tid == 0, 0))
	{
		CacheTid();
	}
	return t_cached_tid;
}
}

#endif //LIVEBROADCASTSERVER_CURRENTTHREAD_H
