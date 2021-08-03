#include <stdlib.h>
#include <stdio.h>

#include "scp/containers/vector.h"
#include "utils.h"

int main(void) {
	struct scpVector* vector = scpVector.new(10, sizeof(size_t));
	for (size_t i = 0; i < vector->count; ++i)
		((size_t*)vector->data)[i] = i;
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.map(vector, square_size_t);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.resize(vector, 17);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.resize(vector, 55);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.resize(vector, 25);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.resize(vector, 3);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	for (size_t i = 0; i < 15; ++i)
		*(size_t*)scpVector_append(vector) = i;
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	for (size_t i = 0; i < 15; ++i)
		scpVector_pop(vector);
	scpVector.print(vector, scpPrint_size_t);
	printf(" %zu / %zu \n", vector->count, vector->reserved);

	scpVector.delete(vector);
	return EXIT_SUCCESS;
}