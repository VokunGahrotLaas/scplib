#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "scp/containers/hashmap.h"
#include "scp/utils.h"

#define BUFFER_SIZE 1024

typedef void (*builtin_func)(size_t argc, char** argv);

typedef struct builtin_pair {
	const char* key;
	builtin_func value;
} builtin_pair;

size_t count_spaces(const char* str);
void make_argv(size_t argc, char** argv, char* buffer);
char* to_lower(const char* str);

void builtin_func_ping(size_t argc, char** argv);
void builtin_func_args(size_t argc, char** argv);
void builtin_func_exit(size_t argc, char** argv);

int main(void) {
	builtin_pair builtin_pairs[] = {
		{ "ping", builtin_func_ping },
		{ "args", builtin_func_args },
		{ "exit", builtin_func_exit }
	};

	struct scpHashMap* builtins = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);

	for (size_t i = 0; i < sizeof builtin_pairs / sizeof *builtin_pairs; ++i)
		scpHashMap.insert(builtins, (void*)builtin_pairs[i].key, (void*)&builtin_pairs[i]);

	char buffer[BUFFER_SIZE] = { 0 }; 

	fputs("Welcome to scp custom shell!\n", stdout);

	while (!feof(stdin)) {
		fputs("\n> ", stdout);
		fgets(buffer, BUFFER_SIZE, stdin);

		size_t argc = count_spaces(buffer) + 1;
		char** argv = (char**)calloc((argc + 1), sizeof(char*));

		make_argv(argc, argv, buffer);
		char* cmd = to_lower(*argv);
		builtin_func func = ((builtin_pair*)scpHashMap.search(builtins, cmd))->value;

		if (func)
			func(argc, argv);
		else
			execve(*argv, argv, __environ);

		free(cmd);
		free(argv);
	}

	scpHashMap.delete(builtins);
	return EXIT_SUCCESS;
}

size_t count_spaces(const char* buffer) {
	size_t count = 0;
	bool last_was_space = true;
	for (; *buffer; ++buffer) {
		if (*buffer == ' ' || *buffer == '\n') {
			if (!last_was_space)
				++count;
			last_was_space = true;
		} else
			last_was_space = false;
	}
	if (last_was_space && count > 0)
		--count;
	return count;
}

void make_argv(size_t argc, char** argv, char* buffer) {
	for (; *buffer == ' '; ++buffer);
	for (size_t i = 0; i < argc; ++i) {
		argv[i] = buffer;
		for (; *buffer && *buffer != ' ' && *buffer != '\n'; ++buffer);
		*buffer++ = '\0';
		for (; *buffer == ' '; ++buffer);
	}
}

char* to_lower(const char* str) {
	char* cpy = strdup(str);
	for (size_t i = 0; cpy[i]; ++i)
		if ('A' <= cpy[i] && cpy[i] <= 'Z')
			cpy[i] += 'a' - 'A';
	return cpy;
}

void builtin_func_ping(scpMacro_unused size_t argc, scpMacro_unused char** argv) {
	fputs("pong!\n", stdout);
}

void builtin_func_args(size_t argc, char** argv) {
	printf("argc: %zu\n", argc);
	fputs("argv: [", stdout);
	if (argc > 0)
		printf(" '%s'", *argv);
	for (size_t i = 1; i < argc; ++i)
		printf(", '%s'", argv[i]);
	if (argc > 0)
		fputc(' ', stdout);
	fputs("]\n", stdout);
}

void builtin_func_exit(scpMacro_unused size_t argc, scpMacro_unused char** argv) {
	exit(EXIT_SUCCESS);
}
