#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "scp/maths/primes.h"

#include "utils.h"

int main(void) {
	for (uint64_t i = 0; i < 16; ++i) {
		printf("next prime to %"PRId64": %"PRId64"\n", i, scpPrimes_next(i));
		scpArray.print(scpPrimes_array, print_uint64_t);
		puts("");
	}
	return EXIT_SUCCESS;
}
