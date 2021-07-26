#ifndef SCP_EXCEPTIONS_H
#define SCP_EXCEPTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdnoreturn.h>

typedef enum scpEsceptionCode {
	scpExceptionCode_Success = EXIT_SUCCESS,
	scpExceptionCode_Failure = EXIT_FAILURE,
	scpExceptionCode_OutOfBound
} scpExitCode;

noreturn void scpException_Exception(int status, const char* message);
noreturn void scpException_Exceptionf(int status, const char* format, ...);
noreturn void scpException_OutOfBound(const char* message);
noreturn void scpException_OutOfBoundf(const char* format, ...);

noreturn void scpException_Exception(int status, const char* message) {
	fprintf(stderr, "scpException (%i): '%s'\n", status, message);
	exit(status);
}

noreturn void scpException_Exceptionf(int status, const char* format, ...) {
	fprintf(stderr, "scpException (%i): '", status);
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("'\n", stderr);
	exit(status);
}


noreturn void scpException_OutOfBound(const char* message) {
	fprintf(stderr, "scpException OutOfBound: '%s'\n", message);
	exit(scpExceptionCode_OutOfBound);
}

noreturn void scpException_OutOfBoundf(const char* format, ...) {
	fputs("scpException OutOfBound: '", stderr);
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("'\n", stderr);
	exit(scpExceptionCode_OutOfBound);
}

#endif // SCP_EXCEPTIONS_H