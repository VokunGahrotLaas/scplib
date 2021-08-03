#ifndef SCP_PRINT_H
#define SCP_PRINT_H

#include <stdio.h>
#include <inttypes.h>

void scpPrint_string(const void* data);
void scpPrint_pointer(const void* data);
void scpPrint_size_t(const void* data);
void scpPrint_int8_t(const void* data);
void scpPrint_uint8_t(const void* data);
void scpPrint_int16_t(const void* data);
void scpPrint_uint16_t(const void* data);
void scpPrint_int32_t(const void* data);
void scpPrint_uint32_t(const void* data);
void scpPrint_int64_t(const void* data);
void scpPrint_uint64_t(const void* data);

void scpPrint_string(const void* data) {
	fputs((const char*)data, stdout);
}

void scpPrint_pointer(const void* data) {
	printf("%p", *(const void**)data);
}

void scpPrint_size_t(const void* data) {
#ifdef SCP_STDC_PRE99
	printf("%lu", *(const size_t*)data);
#else
	printf("%zu", *(const size_t*)data);
#endif
}

void scpPrint_int8_t(const void* data) {
	printf("%"PRIi8, *(const int8_t*)data);
}

void scpPrint_uint8_t(const void* data) {
	printf("%"PRIu8, *(const uint8_t*)data);
}

void scpPrint_int16_t(const void* data) {
	printf("%"PRIi16, *(const int16_t*)data);
}

void scpPrint_uint16_t(const void* data) {
	printf("%"PRIu16, *(const uint16_t*)data);
}

void scpPrint_int32_t(const void* data) {
	printf("%"PRIi32, *(const int32_t*)data);
}

void scpPrint_uint32_t(const void* data) {
	printf("%"PRIu32, *(const uint32_t*)data);
}

void scpPrint_int64_t(const void* data) {
	printf("%"PRIi64, *(const int64_t*)data);
}

void scpPrint_uint64_t(const void* data) {
	printf("%"PRIu64, *(const uint64_t*)data);
}

#endif /* SCP_PRINT_H */