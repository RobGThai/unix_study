#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void ec_warn(void)
{
	fprintf(stderr, "***WARNING: Control flowed into EC_CLEANUP_BGN\n");
}

