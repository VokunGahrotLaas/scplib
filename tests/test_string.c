#include "scp/utils.h"
#include "scp/containers/vector.h"
#include "scp/containers/array.h"
#include "scp/time.h"

#define NB_RUN 100000000
#define MAX_SIZE 100

void strdup_scp(void);
void strdup_cpy(void);
void strdup_rea(void);
void strdup_arr(void);
void strdup_vec(void);
void strndup_scp(void);
char* rand_str(size_t length);
char* scpString_strdup_cpy(const char* str, size_t len);
char* scpString_strdup_rea(const char* str);
char* scpString_strdup_arr(const char* str);
char* scpString_strdup_vec(const char* str);

int main(void) {
	printf("strdup_cpy  : %4lu ms\n", scpTime_ExecTimeMs(strdup_cpy));
	fflush(stdout);
	printf("strdup_scp  : %4lu ms\n", scpTime_ExecTimeMs(strdup_scp));
	fflush(stdout);
	printf("strndup_scp : %4lu ms\n", scpTime_ExecTimeMs(strndup_scp));
	fflush(stdout);
	printf("strdup_vec  : %4lu ms\n", scpTime_ExecTimeMs(strdup_vec));
	fflush(stdout);
	printf("strdup_rea  : %4lu ms\n", scpTime_ExecTimeMs(strdup_rea));
	fflush(stdout);
	printf("strdup_arr  : %4lu ms\n", scpTime_ExecTimeMs(strdup_arr));
	return EXIT_SUCCESS;
}

void strdup_scp(void) {
	srand((unsigned)time(NULL));
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		char* str = rand_str((size_t)rand() % MAX_SIZE);
		char* cpy = scpString_strdup(str);
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

void strdup_cpy(void) {
	srand((unsigned)time(NULL));
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		size_t len = (size_t)rand() % MAX_SIZE;
		char* str = rand_str(len);
		char* cpy = scpString_strdup_cpy(str, len);
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

void strdup_rea(void) {
	srand((unsigned)time(NULL));
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		char* str = rand_str((size_t)rand() % MAX_SIZE);
		char* cpy = scpString_strdup_rea(str);
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

void strdup_arr(void) {
	srand((unsigned)time(NULL));
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		char* str = rand_str((size_t)rand() % MAX_SIZE);
		char* cpy = scpString_strdup_arr(str);
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

void strdup_vec(void) {
	srand((unsigned)time(NULL));
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		char* str = rand_str((size_t)rand() % MAX_SIZE);
		char* cpy = scpString_strdup_vec(str);
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

void strndup_scp(void) {
	FILE* devnull = fopen("/dev/null", "a");
	for (size_t i = 0; i < NB_RUN; ++i) {
		char* str = rand_str((size_t)rand() % MAX_SIZE);
		char* cpy = scpString_strndup(str, strlen(str));
		free(str);
		fputs(cpy, devnull);
		free(cpy);
	}
	fclose(devnull);
}

char* rand_str(size_t length) {
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	char* str = malloc((length + 1) * sizeof(char));
	if (!str) return str;
	const int mod = sizeof charset / sizeof *charset - 1;
	str[length] = '\0';
	for (size_t i = 0; i < length; ++i)
		str[i] = charset[rand() % mod];
	return str;
}

char* scpString_strdup_cpy(const char* str, size_t len) {
	char* cpy = malloc((len + 1) * sizeof(char));
	strcpy(cpy, str);
	return cpy;
}

char* scpString_strdup_rea(const char* str) {
	char* cpy = malloc(sizeof(char));
	size_t i;
	for (i = 0; str[i]; ++i) {
		cpy = realloc(cpy, (i + 2) * sizeof(char));
		cpy[i] = str[i];
	}
	cpy[i] = '\0';
	return cpy;
}

char* scpString_strdup_arr(const char* str) {
	struct scpArray* arr = scpArray_new(1, sizeof(char));
	for (; *str; ++str)
		*(char*)scpArray_append(arr) = *str;
	*(char*)scpArray_append(arr) = '\0';
	char* cpy = malloc(0);
	SCP_SWAP(char*, arr->data, cpy);
	scpArray_delete(arr);
	return cpy;
}

char* scpString_strdup_vec(const char* str) {
	struct scpVector* vec = scpVector_new(1, sizeof(char));
	for (; *str; ++str)
		*(char*)scpVector_append(vec) = *str;
	char* cpy = malloc(0);
	SCP_SWAP(char*, vec->data, cpy);
	scpVector_delete(vec);
	return cpy;
}
