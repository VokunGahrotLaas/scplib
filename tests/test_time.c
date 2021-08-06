#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scp/time.h"
#include "scp/containers/array.h"

#include "utils.h"

void func_to_time(void);
void assign_index(void* data, size_t index, size_t count);

int main(void) {
	printf("time: %lims\n", scpTime_ExecTimeMs(func_to_time));
	return EXIT_SUCCESS;
}

void func_to_time(void) {
	struct scpArray* array = scpArray.new(1000000, sizeof(size_t));
	scpArray.map_index(array, assign_index);
	scpArray.map(array, square_size_t);
	FILE* devnull = fopen("/dev/null", "a");
	scpArray.fprint(array, devnull, scpFPrint_size);
	fclose(devnull);
}

void assign_index(void* data, size_t index, scpAttribute_unused size_t count) {
	*(size_t*)data = (size_t)index;
}
