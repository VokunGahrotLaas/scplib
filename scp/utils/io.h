#ifndef SCP_IO_H
#define SCP_IO_H

#include <stdio.h>

#include "scp/utils/macros.h"

scpBool scpIO_flush_buffer(FILE* stream, char* buffer);

#ifdef SCP_IMPLEMENTATION

scpBool scpIO_flush_buffer(FILE* stream, char* buffer) {
	char* endl = strchr(buffer, '\n');
	if (!endl)
		while (!feof(stream) && fgetc(stream) != '\n');
	else
		*endl = '\0';
	return !endl ? scpTrue : scpFalse;
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_IO_H
