#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <pwd.h>

#include "scp/containers/hashmap.h"
#include "scp/utils.h"

#ifndef SCP_SHELL_INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE (1 << 20)
#else
#define INPUT_BUFFER_SIZE SCP_SHELL_INPUT_BUFFER_SIZE
#endif

typedef int (*builtin_func)(size_t argc, char** argv);

typedef struct builtin_pair {
	const char* key;
	builtin_func value;
} builtin_pair;

static volatile sig_atomic_t sigint_recieved = 0;
void signal_handler(int signo);

size_t get_argc(const char* str);
void make_argv(size_t argc, char** argv, char* buffer);
char* to_lower(const char* str);
bool flush_buffer(FILE* stream, char* buffer);

int builtin_func_ping(size_t argc, char** argv);
int builtin_func_args(size_t argc, char** argv);
int builtin_func_exit(size_t argc, char** argv);
int builtin_func_cd(size_t argc, char** argv);
int builtin_func_shsleep(size_t argc, char** argv);

int main(scpAttribute_unused int argc, char** argv, char** env) {
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		SCP_EXCEPTION(scpException_Exception, "signal handler failed");

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
		{ "exit", builtin_func_exit },
		{ "cd", builtin_func_cd },
		{ "shsleep", builtin_func_shsleep }
	};

	struct scpHashMap* builtins = scpHashMap.new(scpHash_stringA, scpHash_stringB, scpCmp_string);

	for (size_t i = 0; i < sizeof builtin_pairs / sizeof *builtin_pairs; ++i)
		scpHashMap.insert(builtins, (void*)builtin_pairs[i].key, (void*)&builtin_pairs[i]);

	char buffer[INPUT_BUFFER_SIZE];

	while (!feof(stdin)) {
		fputs("$ ", stdout);
		fgets(buffer, INPUT_BUFFER_SIZE, stdin);

		if (sigint_recieved) {
			sigint_recieved = 0;
			fputc('\n', stdout);
			continue;
		}

		if (flush_buffer(stdin, buffer)) {
			fprintf(stderr, "%s: input was too long to fit in the buffer\n", *argv);
			continue;
		}

		if (*buffer == '\0')
			continue;

		size_t new_argc = get_argc(buffer);
		char** new_argv = (char**)calloc((new_argc + 1), sizeof(char*));

		make_argv(new_argc, new_argv, buffer);
		char* cmd = to_lower(*new_argv);
		builtin_pair* pair = (builtin_pair*)scpHashMap.search(builtins, cmd);

		if (pair) {
			status = pair->value(new_argc, new_argv);
			if (status)
				printf("proccess exited with code %i\n", status);
		} else if ((pid = fork()) == 0) {
			if (execve(*new_argv, new_argv, __environ) == -1) {
				switch (errno) {
					case EINVAL:
					case EACCES:
					case ELOOP:
					case ETXTBSY:
					case ENOEXEC:
					case ENOTDIR:
					case ENOENT:
						break;
					case E2BIG:
						fprintf(stderr, "%s: argument list too long\n", *argv);
						return EXIT_FAILURE;
					case ENAMETOOLONG:
						fprintf(stderr, "%s: file name too long\n", *argv);
						return EXIT_FAILURE;
					case ENOMEM:
						fprintf(stderr, "%s: out of memory\n", *argv);
						return EXIT_FAILURE;
					default:
						fprintf(stderr, "%s: unknown error: %s\n", *argv, *new_argv);
						return EXIT_FAILURE;
				}
			}
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
				execve(cmd, new_argv, env);
			}
			fprintf(stderr, "%s: command not found: %s\n", *argv, old_cmd);
			free(old_cmd);
			exit(EXIT_FAILURE);
		} else if (pid == -1)
			SCP_EXCEPTION(scpException_Exception, "failed to fork");
		else {
			if (waitpid(pid, &status, 0) == -1) {
				switch (errno) {
					case EINTR:
						break;
					case ECHILD:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, no child proccess");
						break;
					case EINVAL:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, no child proccess");
						break;
					default:
						SCP_EXCEPTION(scpException_Exception, "failed to waitpid, unknown error");
						break;
				}
			}
			if (WEXITSTATUS(status))
				printf("proccess exited with code %i\n", WEXITSTATUS(status));
			if (WIFSIGNALED(status))
				WTERMSIG(status);
		}

		if (sigint_recieved == 1) {
			sigint_recieved = 0;
			fputs("proccess was killed\n", stdout);
		}

		free(cmd);
		free(new_argv);
	}

	scpHashMap.delete(builtins);
	free(path);
	return EXIT_SUCCESS;
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

size_t get_argc(const char* buffer) {
	size_t count = 1;
	bool last_was_space = true;
	for (; *buffer; ++buffer) {
		if (*buffer == ' ') {
			if (!last_was_space)
				++count;
			last_was_space = true;
		} else
			last_was_space = false;
	}
	if (last_was_space && count > 1)
		--count;
	return count;
}

void make_argv(size_t argc, char** argv, char* buffer) {
	for (; *buffer == ' '; ++buffer);
	for (size_t i = 0; i < argc; ++i) {
		argv[i] = buffer;
		for (; *buffer && *buffer != ' '; ++buffer);
		if (!*buffer) break;
		*buffer++ = '\0';
		for (; *buffer == ' '; ++buffer);
	}
	argv[argc] = NULL;
}

char* to_lower(const char* str) {
	char* cpy = strdup(str);
	for (char* p = cpy; *p; ++p)
		if ('A' <= *p && *p <= 'Z')
			*p += 'a' - 'A';
	return cpy;
}

bool flush_buffer(FILE* stream, char* buffer) {
	char* endl = strchr(buffer, '\n');
	if (!endl)
		while (!feof(stream) && fgetc(stream) != '\n');
	else
		*endl = '\0';
	return !endl;
}

int builtin_func_ping(scpAttribute_unused size_t argc, scpAttribute_unused char** argv) {
	fputs("pong!\n", stdout);
	return EXIT_SUCCESS;
}

int builtin_func_args(size_t argc, char** argv) {
	printf("argc: %zu\n", argc);
	fputs("argv: [", stdout);
	if (argc > 0)
		printf(" '%s'", *argv);
	for (size_t i = 1; i < argc; ++i)
		printf(", '%s'", argv[i]);
	if (argc > 0)
		fputc(' ', stdout);
	fputs("]\n", stdout);
	return EXIT_SUCCESS;
}

int builtin_func_exit(scpAttribute_unused size_t argc, scpAttribute_unused char** argv) {
	exit(EXIT_SUCCESS);
}

int builtin_func_cd(size_t argc, char** argv) {
	if (argc < 2) {
		fputs("cd: not yet dude\n", stdout);
		return EXIT_SUCCESS;
	}
	if (argc > 2) {
		fputs("cd: too many arguments\n", stdout);
		return EXIT_FAILURE;
	}
	if (chdir(argv[1]) == 0)
		return EXIT_SUCCESS;
	switch (errno) {
		case EINTR:
			break;
		case EACCES:
			fputs("cd: search permission has been denied\n", stderr);
			break;
		case ELOOP:
			fputs("cd: too many symbolic links were encountered\n", stderr);
			break;
		case ENAMETOOLONG:
			fputs("cd: path is too large (PATH_MAX: "SCP_TO_STRING(PATH_MAX)")\n", stderr);
			break;
		case ENOENT:
			fprintf(stderr, "cd: no such file or directory: %s\n", argv[1]);
			break;
		case ENOTDIR:
			fprintf(stderr, "cd: not a directory: %s\n", argv[1]);
			break;
		default:
			fputs("cd: unknown error\n", stderr);
			break;
	}
	return errno;
}

int builtin_func_shsleep(size_t argc, char** argv) {
	if (argc < 2) {
		fputs("sleep: too few arguments\n", stdout);
		return EXIT_FAILURE;
	}
	for (size_t i = 1; i < argc; ++i) {
		unsigned to_sleep;
		if (sscanf(argv[i], "%u", &to_sleep) != 1) {
			fprintf(stderr, "sleep: invalid argument: %s", argv[i]);
			return EXIT_FAILURE;
		}
		if (sleep(to_sleep) != 0)
			return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
