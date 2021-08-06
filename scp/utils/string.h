#ifndef SCP_STRING_H
#define SCP_STRING_H

#include "scp/utils/macros.h"

#ifdef SCP_ALLOC_LIB

scpAttribute_malloc
char* scpString_strdup(const char *str);
scpAttribute_malloc
char* scpString_strndup(const char *str, size_t size);

#else

#define scpString_strdup strdup
#define scpString_strndup strndup

#endif

char* scpString_to_lower(char* str);
char* scpString_to_upper(char* str);

scpAttribute_malloc
char* scpString_new_lower(const char* str);
scpAttribute_malloc
char* scpString_new_upper(const char* str);

#ifdef SCP_IMPLEMENTATION

#ifdef SCP_ALLOC_LIB

char* scpString_strdup(const char* str) {
	size_t str_len = strlen(str);
	char* cpy = malloc((str_len + 1) * sizeof(char));
	for (size_t i = 0; i < str_len; ++i)
		cpy[i] = str[i];
	cpy[str_len] = '\0';
	return cpy;
}

char* scpString_strndup(const char* str, size_t size) {
	char* cpy = malloc((size + 1) * sizeof(char));
	size_t i;
	for (i = 0; i < size && str[i]; ++i)
		cpy[i] = str[i];
	cpy[i] = '\0';
	if (i < size)
		cpy = realloc(cpy, (i + 1) * sizeof(char));
	return cpy;
}

#endif

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
	char* cpy = scpString_strdup(str);
	return scpString_to_lower(cpy);
}

char* scpString_new_upper(const char* str) {
	char* cpy = scpString_strdup(str);
	return scpString_to_upper(cpy);
}

#endif // SCP_IMPLEMENTATION

#endif // SCP_STRING_H
