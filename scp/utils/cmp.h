#ifndef SCP_CMP_H
#define SCP_CMP_H

#include <stdlib.h>
#include <string.h>

long scpCmp_string(const void* a, const void* b) {
	return strcmp((const char*)a, (const char*)b);
}

#endif /* SCP_CMP_H */