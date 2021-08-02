#ifndef SCP_PRINT_H
#define SCP_PRINT_H

#include <stdio.h>
#include <inttypes.h>

void scpPrint_string(const void* data);
void scpPrint_uint64_t(const void* data);
void scpPrint_size_t(const void* data);

void scpPrint_string(const void* data) {
	fputs((const char*)data, stdout);
}

void scpPrint_uint64_t(const void* data) {
	printf("%"PRId64, *(const uint64_t*)data);
}

void scpPrint_size_t(const void* data) {
	printf("%zu", *(const size_t*)data);
}

#endif // SCP_PRINT_H