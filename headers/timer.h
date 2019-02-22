#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
	extern "C" {
#endif

unsigned long long getTicks();
long long randomValue(long long min, long long max);
int compareTimer(unsigned long long tick);

#ifdef __cplusplus
	}
#endif

#endif //__TIMER_H__
