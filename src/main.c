#include <unistd.h>
#include <stdio.h>
#include "main.h"

int	main(int ac, char **av)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		perror("pipe :");
	if (fork() == 0)
	{
		//2 --> stdout
		//pipefd[1] --> pipe_write_end
		close(pipefd[0]);
		dup2(pipefd[1], 2);
		close(pipefd[1]);
		// fd 2 now points to our pipe's write end
		// Then we can exec our binary that uses 2 for its output
		execv(av[0], av);
	}

	return (0);
}
