#include <stdlib.h>
#include <stdio.h>

#include "scp/maths.h"

int main(void) {
	int a = 5, b = 3;
	printf("%i ** %i = %i\n", a, b, scpMaths_pow(a, b));
	return EXIT_SUCCESS;
}
