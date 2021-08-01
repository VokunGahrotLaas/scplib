#include <stdlib.h>
#include <stdio.h>

#include "scp/int128.h"

int main(void) {
	scp_int128_t i = (scp_int128_t)-1;
	scpUtils_print_int128(i);
	puts("");

	scp_uint128_t u = (scp_uint128_t)i;
	scpUtils_print_uint128(u);
	puts("");
}
