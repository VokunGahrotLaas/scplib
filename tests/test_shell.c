#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ncurses.h>

#include "scp/containers/hashmap.h"
#include "scp/utils.h"

#define BUFFER_SIZE (1 << 20)

typedef void (*builtin_func)(size_t argc, char** argv);

typedef struct builtin_pair {
	const char* key;
	builtin_func value;
} builtin_pair;

scpMacro_constructor void ncurses_constructor(void);
scpMacro_destructor void ncurses_desctructor(void);

static volatile sig_atomic_t sigint_recieved = 0;
void signal_handler(int signo);

size_t count_spaces(const char* str);
void make_argv(size_t argc, char** argv, char* buffer);
char* to_lower(const char* str);

void builtin_func_ping(size_t argc, char** argv);
void builtin_func_args(size_t argc, char** argv);
void builtin_func_exit(size_t argc, char** argv);

int main(void) {
	if (signal(SIGINT, signal_handler) == SIG_ERR) SCP_EXCEPTION(scpException_Exception, "signal handler failed");

	pid_t pid;
	int status;

	char* path = strdup(getenv("PATH"));
	if (!path) SCP_EXCEPTION(scpException_Exception, "no path variable in env");
	if (*path == '\0') SCP_EXCEPTION(scpException_Exception, "empty path variable in env");

	char** pathv = (char**)calloc(2, sizeof(char*));
	*pathv = path;
	size_t pathc = 1;
	for (size_t i = 0; path[i]; ++i) {
		if (path[i] == ':') {
			pathv = (char**)realloc(pathv, (++pathc + 1) * sizeof(char*));
			pathv[pathc - 1] = path + i + 1;
			pathv[pathc] = NULL;
			path[i] = '\0';
		}
	}

	builtin_pair builtin_pairs[] = {
		{ "ping", builtin_func_ping },
		{ "args", builtin_func_args },
		{ "exit", builtin_func_exit }
	};

	struct scpHashMap* builtins = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);

	for (size_t i = 0; i < sizeof builtin_pairs / sizeof *builtin_pairs; ++i)
		scpHashMap.insert(builtins, (void*)builtin_pairs[i].key, (void*)&builtin_pairs[i]);

	char buffer[BUFFER_SIZE];

	fputs("Welcome to scp custom shell!\n", stdout);

	while (!feof(stdin)) {
		fputs("\n$ ", stdout);
		fgets(buffer, BUFFER_SIZE, stdin);

		if (sigint_recieved) {
			sigint_recieved = 0;
			printf("mean sigint\n");
			continue;
		}

		size_t new_argc = count_spaces(buffer) + 1;
		char** new_argv = (char**)calloc((new_argc + 1), sizeof(char*));

		make_argv(new_argc, new_argv, buffer);
		char* cmd = to_lower(*new_argv);
		builtin_pair* pair = (builtin_pair*)scpHashMap.search(builtins, cmd);

		if (pair)
			pair->value(new_argc, new_argv);
		else if ((pid = fork()) == 0) {
			execve(*new_argv, new_argv, __environ);
			char* old_cmd = strdup(*new_argv);
			size_t old_cmd_len = strlen(old_cmd);
			for (size_t i = 0; i < pathc; ++i) {
				size_t path_len = (size_t)strlen(pathv[i]);
				cmd = realloc(cmd, (path_len + old_cmd_len + 2) * sizeof(char));
				for (size_t j = 0; j < path_len; ++j)
					cmd[j] = pathv[i][j];
				cmd[path_len] = '/';
				for (size_t j = 0; j < old_cmd_len; ++j)
					cmd[path_len + 1 + j] = old_cmd[j];
				cmd[path_len + 1 + old_cmd_len] = '\0';
				*new_argv = cmd;
				execve(*new_argv, new_argv, __environ);
			}
			free(old_cmd);
			fputs("invalid command\n", stdout);
			exit(EXIT_FAILURE);
		}

		wait(&status);
		if (status)
			printf("exited with code %i", status);

		free(cmd);
		free(new_argv);
	}

	scpHashMap.delete(builtins);
	free(path);
	return EXIT_SUCCESS;
}

scpMacro_constructor void ncurses_constructor(void) {
	//initscr();
}

scpMacro_destructor void ncurses_desctructor(void) {
	//endwin();
}

void signal_handler(int signo) {
	switch (signo) {
		case SIGINT:
			sigint_recieved = 1;
			break;
		default:
			SCP_EXCEPTION(scpException_Exception, "unhandled signal");
	}
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
