#ifndef SCP_STRING_H
#define SCP_STRING_H

#include <string.h>

char* scpString_to_lower(char* str);
char* scpString_to_upper(char* str);

char* scpString_new_lower(const char* str);
char* scpString_new_upper(const char* str);

#ifdef SCP_IMPLEMENTATION

char* scpString_to_lower(char* str) {
	for (char* p = str; *p; ++p)
		if ('A' <= *p && *p <= 'Z')
			*p += 'a' - 'A';
	return str;
}

char* scpString_to_upper(char* str) {
	for (char* p = str; *p; ++p)
		if ('a' <= *p && *p <= 'z')
			*p -= 'a' - 'A';
	return str;
}

char* scpString_new_lower(const char* str) {
	char* cpy = strdup(str);
	return scpString_to_lower(cpy);
}

char* scpString_new_upper(const char* str) {
	char* cpy = strdup(str);
	return scpString_to_upper(cpy);
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_STRING_H
