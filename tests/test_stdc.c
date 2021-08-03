#include <stdlib.h>
#include <stdio.h>

#include "scp/utils.h"
#include "scp/maths/binpow.h"
#include "scp/maths/primes.h"
#include "scp/containers/array.h"
#include "scp/containers/vector.h"
#include "scp/containers/hashmap.h"
#include "scp/containers/hashset.h"
#include "scp/exceptions.h"
#include "scp/time.h"
#include "scp/io.h"
#include "scp/int128.h"

int main(void) {
#ifdef __STDC_VERSION__
	printf("__STCD_VERSION__: %li\n", __STDC_VERSION__);
#else
	printf("__STCD_VERSION__: (%p)\n", NULL);
#endif
	return EXIT_SUCCESS;
}
