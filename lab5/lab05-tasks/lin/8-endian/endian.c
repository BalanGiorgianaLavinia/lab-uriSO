/**
  * SO, 2016
  * Lab #5
  *
  * Task #8, lin
  *
  * Endianess
  */
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int main(void)
{
	int i;
	unsigned int n = 0xDEADBEEF;
	unsigned char *w;

	/* TODO - use w to show all bytes of n in order */
  w = &n;
  for (i = 0; i < sizeof(n); i++)
    printf("%x", w[i]);

	return 0;
}

