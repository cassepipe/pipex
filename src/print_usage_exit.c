#include <unistd.h>
#include <stdlib.h>

static const char	g_usage[] = {
	"Usage : "
	"./pipex infile \"cmd_1 [--option]...\" \"cmd_2 [--option]...\" outfile\n"
};

void	print_usage_exit(void)
{
	write(STDOUT_FILENO, g_usage, sizeof(g_usage));
	exit(EXIT_FAILURE);
}
