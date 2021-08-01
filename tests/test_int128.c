#include <stdlib.h>
#include <stdio.h>

#ifdef SCP_PEDANTIC

int main(void) {
	puts("This test is irrelevant with -Wpedantic");
	return EXIT_SUCCESS;
}

#else

#include "scp/int128.h"

int main(void) {
	scp_int128_t i = (scp_int128_t)-1;
	scp_print_int128(i);
	puts("");

	scp_uint128_t u = (scp_uint128_t)i;
	scp_print_uint128(u);
	puts("");
	
	return EXIT_SUCCESS;
}

#endif
