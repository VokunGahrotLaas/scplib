#ifndef SCP_TIME_H
#define SCP_TIME_H

#include "scp/utils/macros.h"

clock_t scpTime_clocksToNs(clock_t t);
clock_t scpTime_clocksToMs(clock_t t);
clock_t scpTime_clocksToS(clock_t t);
clock_t scpTime_ExecTime(void (*func)(void));
clock_t scpTime_ExecTimeNs(void (*func)(void));
clock_t scpTime_ExecTimeMs(void (*func)(void));
clock_t scpTime_ExecTimeS(void (*func)(void));

#ifdef SCP_IMPLEMENTATION

clock_t scpTime_clocksToNs(clock_t t) {
	return (clock_t)((size_t)t * 1000000 / CLOCKS_PER_SEC);
}

clock_t scpTime_clocksToMs(clock_t t) {
	return (clock_t)((size_t)t * 1000 / CLOCKS_PER_SEC);
}

clock_t scpTime_clocksToS(clock_t t) {
	return t / CLOCKS_PER_SEC;
}

clock_t scpTime_ExecTime(void (*func)(void)) {
	clock_t t = clock();
	func();
	return clock() - t;
}

clock_t scpTime_ExecTimeNs(void (*func)(void)) {
	return scpTime_clocksToNs(scpTime_ExecTime(func));
}

clock_t scpTime_ExecTimeMs(void (*func)(void)) {
	return scpTime_clocksToMs(scpTime_ExecTime(func));
}

clock_t scpTime_ExecTimeS(void (*func)(void)) {
	return scpTime_clocksToS(scpTime_ExecTime(func));
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_TIME_H
