#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../libft/libft.h"
#include <linux/limits.h>
#include <unistd.h>
#include "getvar.h"
#include <errno.h>

char	g_buffer[PATH_MAX];

Test(access, x_ok)
{
	char makeheaders[] = "/home/bajaba/Desktop/pipex/makeheaders";
	cr_expect(access(makeheaders, X_OK) == -1);
	cr_log_warn(strerror(errno));
}


void redirect_all_stdout(void)
{
	cr_redirect_stdout();
	cr_redirect_stderr();
}

int	error(void)
{
	write(2, "error", 5);
	return (0);
}

Test(errors, exit_code, .init = redirect_all_stdout)
{
	error();
	cr_assert_stderr_eq_str("error", "");
}
