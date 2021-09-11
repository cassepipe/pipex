#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../libft/libft.h"


Test(sample, test) {
    cr_expect(ft_strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
    cr_expect(ft_strlen("Hello") == 5, "This must pass");
    cr_expect(ft_strlen("") == 0);
}

void redirect_all_stdout(void)
{
        cr_redirect_stdout();
        cr_redirect_stderr();
}

int error(void)
{
        write(2, "error", 5);
        return(0);
}

Test(errors, exit_code, .init=redirect_all_stdout)
{
        error();
        cr_assert_stderr_eq_str("error", "");
}
