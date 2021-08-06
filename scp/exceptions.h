#ifndef SCP_EXCEPTIONS_H
#define SCP_EXCEPTIONS_H

#include "scp/utils/macros.h"

#define _SCP_MAKE_NEW_EXCEPTION(name) \
scpAttribute_format_printf(4, 5) scpNoreturn \
void scpException_##name(const char* file, int line, const char* func, const char* format, ...);

_SCP_MAKE_NEW_EXCEPTION(Exception)
_SCP_MAKE_NEW_EXCEPTION(OutOfBound)
_SCP_MAKE_NEW_EXCEPTION(NullPointer)
_SCP_MAKE_NEW_EXCEPTION(NotImplemented)
_SCP_MAKE_NEW_EXCEPTION(InvalidArgument)

#undef _SCP_MAKE_NEW_EXCEPTION

#define SCP_EXCEPTION(name, ...) name(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef SCP_IMPLEMENTATION

#define _SCP_MAKE_NEW_EXCEPTION(name) \
void scpException_##name(const char* file, int line, const char* func, const char* format, ...) { \
	fprintf(stderr, "scpException "SCP_TO_STRING(name)" - \"%s\" line %i in %s: \"", file, line, func); \
	va_list args; \
	va_start(args, format); \
	vfprintf(stderr, format, args); \
	va_end(args); \
	fputs("\"\n", stderr); \
	abort(); \
}

_SCP_MAKE_NEW_EXCEPTION(Exception)
_SCP_MAKE_NEW_EXCEPTION(OutOfBound)
_SCP_MAKE_NEW_EXCEPTION(NullPointer)
_SCP_MAKE_NEW_EXCEPTION(NotImplemented)
_SCP_MAKE_NEW_EXCEPTION(InvalidArgument)

#undef _SCP_MAKE_NEW_EXCEPTION

#endif // SCP_IMPLEMENTATION

#endif // SCP_EXCEPTIONS_H
