#include <criterion/criterion.h>

#include "progressbar.h"
#include "../src/progressbar.h"

Test(read_progressbar, valid_single_digit)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[3/6] ", &bar), 0);
    cr_assert_eq(bar.cur, 3);
    cr_assert_eq(bar.max, 6);
}

Test(read_progressbar, valid_two_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12/34] ", &bar), 0);
    cr_assert_eq(bar.cur, 12);
    cr_assert_eq(bar.max, 34);
}

Test(read_progressbar, valid_three_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[123/456] ", &bar), 0);
    cr_assert_eq(bar.cur, 123);
    cr_assert_eq(bar.max, 456);
}

Test(read_progressbar, valid_four_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[1234/5678] ", &bar), 0);
    cr_assert_eq(bar.cur, 1234);
    cr_assert_eq(bar.max, 5678);
}

Test(read_progressbar, valid_five_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12345/60789] ", &bar), 0);
    cr_assert_eq(bar.cur, 12345);
    cr_assert_eq(bar.max, 60789);
}

Test(read_progressbar, valid_zeros)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[0/0] ", &bar), 0);
    cr_assert_eq(bar.cur, 0);
    cr_assert_eq(bar.max, 0);
}

Test(read_progressbar, valid_variable_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12/3456] ", &bar), 0);
    cr_assert_eq(bar.cur, 12);
    cr_assert_eq(bar.max, 3456);
}

Test(read_progressbar, fail_max_short)
{
    struct progressbar_t bar = {
            .max = 5,
    };
    cr_assert_eq(read_progressbar("[123/654321] ", &bar), 3);
    cr_assert_eq(bar.cur, 123);
    cr_assert_eq(bar.max, 5);
}