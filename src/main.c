#include <unistd.h>
#include <stdio.h>
#include "main.h"

int main(int ac, char **av)
{
	printf("%s\n", av[1]);
	printf("%s\n", av[2]);

	return (0);
}
