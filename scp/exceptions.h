#ifndef SCP_EXCEPTIONS_H
#define SCP_EXCEPTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdnoreturn.h>

#define SCP_MAKE_EXCEPTION(name, code, func, funcf)								\
noreturn void func(const char* message) {										\
	fprintf(stderr, "scpException %s: '%s'\n", name, message);					\
	exit(code);																	\
}																				\
																				\
noreturn void funcf(const char* format, ...) {									\
	fprintf(stderr, "scpException %s: '", name);								\
	va_list args;																\
	va_start(args, format);														\
	vfprintf(stderr, format, args);												\
	va_end(args);																\
	fputs("'\n", stderr);														\
	exit(code);																	\
}

typedef enum scpEsceptionCode {
	scpExceptionCode_Success = EXIT_SUCCESS,
	scpExceptionCode_Failure = EXIT_FAILURE,
	scpExceptionCode_OutOfBound,
	scpExceptionCode_NullPointer,
	scpExceptionCode_NotImplemented,
	scpExceptionCode_InvalidArgument
} scpExitCode;

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

SCP_MAKE_EXCEPTION("OutOfBound", scpExceptionCode_OutOfBound, scpException_OutOfBound, scpException_OutOfBoundf)
SCP_MAKE_EXCEPTION("NullPointer", scpExceptionCode_NullPointer, scpException_NullPointer, scpException_NullPointerf)
SCP_MAKE_EXCEPTION("NotImplemented", scpExceptionCode_NotImplemented, scpException_NotImplemented, scpException_NotImplementedf)
SCP_MAKE_EXCEPTION("InvalidArgument", scpExceptionCode_InvalidArgument, scpException_InvalidArgument, scpException_InvalidArgumentf)

#endif // SCP_EXCEPTIONS_H