#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/array.h"
#include "utils.h"

int main(void) {
	struct scpArray* array = scpArray.new(10, sizeof(size_t));
	for (size_t i = 0; i < array->count; ++i)
		((size_t*)array->data)[i] = i;
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.map(array, square_size_t);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	scpArray.resize(array, 16);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	struct scpArray* clone = scpArray_clone(array);
	struct scpArray* fclone = scpArray_fclone(array, copy_size_t);

	printf("array:  ");
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	printf("clone:  ");
	scpArray.print(clone, print_size_t);
	putc('\n', stdout);

	printf("fclone: ");
	scpArray.print(fclone, print_size_t);
	putc('\n', stdout);

	scpArray.resize(array, 4);
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	printf("array:  ");
	scpArray.print(array, print_size_t);
	putc('\n', stdout);

	printf("clone:  ");
	scpArray.print(clone, print_size_t);
	putc('\n', stdout);

	printf("fclone: ");
	scpArray.print(fclone, print_size_t);
	putc('\n', stdout);

	scpArray.delete(array);
	scpArray.delete(clone);
	scpArray.delete(fclone);
	return EXIT_SUCCESS;
}