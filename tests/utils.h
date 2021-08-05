#ifndef SCP_TESTS_UTILS_H
#define SCP_TESTS_UTILS_H

#include "scp/utils.h"

void square_size_t(void* data);

void square_size_t(void* data) {
	*(size_t*)data *= *(size_t*)data;
}

#endif // SCP_TESTS_UTILS_H
