#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "scp/maths/primes.h"

#include "utils.h"

#define VALUE (1 << 16)

int main(void) {
	for (uint64_t i = 0; i < 16; ++i) {
		printf("next prime to %"PRId64": %"PRId64"\n", i, scpPrimes_next(i));
		scpVector.print(scpPrimes_vector, scpPrint_uint64);
		printf("\n");
	}

	printf("next to "SCP_TO_STRING(VALUE)": %"PRIu64"\n", scpPrimes_next(VALUE));
	printf("next to next: %"PRIu64"\n", scpPrimes_next(scpPrimes_next(VALUE) + 1));

	return EXIT_SUCCESS;
}
