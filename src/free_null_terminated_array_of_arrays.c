#include <stdlib.h>

void	free_null_terminated_array_of_arrays(char **array)
{
	int i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
