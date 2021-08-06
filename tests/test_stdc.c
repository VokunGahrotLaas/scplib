#include "scp/utils.h"
#include "scp/maths.h"
#include "scp/containers/array.h"
#include "scp/containers/vector.h"
#include "scp/containers/hashmap.h"
#include "scp/containers/hashset.h"
#include "scp/exceptions.h"
#include "scp/time.h"
#include "scp/stream.h"
#include "scp/int128.h"

int main(void) {
#ifdef __STDC_VERSION__
	printf("__STCD_VERSION__: %li\n", __STDC_VERSION__);
#else
	printf("__STCD_VERSION__: (nil)\n");
#endif
	return EXIT_SUCCESS;
}
