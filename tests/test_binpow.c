#include <stdlib.h>
#include <stdio.h>

#include "scp/maths/binpow.h"

int main(void) {
	int a = 5, b = 3;
	printf("%i ** %i = %i\n", a, b, scpMaths_binpow(a, b));
	return EXIT_SUCCESS;
}
