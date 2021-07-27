#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/array.h"
#include "utils.h"

int main(void) {
	struct scpArray* array = scpArray.new(10, sizeof(size_t));
	for (size_t i = 0; i < array->count; ++i)
		*(size_t*)scpArray.at(array, i) = i;
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.map(array, square);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.resize(array, 16);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.resize(array, 4);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.delete(array);
	return EXIT_SUCCESS;
}