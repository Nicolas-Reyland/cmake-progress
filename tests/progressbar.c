#include <criterion/criterion.h>

#include <limits.h>

#include "progressbar.h"
#include "../src/progressbar.h"

#define STRINGIFY(x) #x

Test(read_progressbar, valid_single_digit)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[3/6] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 3);
    cr_assert_eq(bar.max, 6);
}

Test(read_progressbar, valid_two_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12/34] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 12);
    cr_assert_eq(bar.max, 34);
}

Test(read_progressbar, valid_three_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[123/456] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 123);
    cr_assert_eq(bar.max, 456);
}

Test(read_progressbar, valid_four_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[1234/5678] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 1234);
    cr_assert_eq(bar.max, 5678);
}

Test(read_progressbar, valid_five_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12345/60789] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 12345);
    cr_assert_eq(bar.max, 60789);
}

Test(read_progressbar, valid_zeros)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[0/0] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 0);
    cr_assert_eq(bar.max, 0);
}

Test(read_progressbar, valid_variable_digits)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[12/3456] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 12);
    cr_assert_eq(bar.max, 3456);
}

Test(read_progressbar, fail_cur_negative)
{
    struct progressbar_t bar = {
            .cur = 4,
            .max = 5,
    };
    cr_assert_eq(read_progressbar("[-123/3] ", &bar, NULL), 1);
    cr_assert_eq(bar.cur, 4);
    cr_assert_eq(bar.max, 5);
}

Test(read_progressbar, fail_max_negative)
{
    struct progressbar_t bar = {
            .max = 5,
    };
    cr_assert_eq(read_progressbar("[4/-3] ", &bar, NULL), 3);
    cr_assert_eq(bar.cur, 4);
    cr_assert_eq(bar.max, 5);
}

Test(read_progressbar, fail_exceed_max_short)
{
    struct progressbar_t bar = {
            .max = 5,
    };
    cr_assert_eq(read_progressbar("[123/654321] ", &bar, NULL), 3);
    cr_assert_eq(bar.cur, 123);
    cr_assert_eq(bar.max, 5);
}

Test(read_progressbar, valid_cur_more_than_max)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[5/3] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 5);
    cr_assert_eq(bar.max, 3);
}

Test(read_progressbar, valid_two_max_shorts)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[65535/65535] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, USHRT_MAX);
    cr_assert_eq(bar.max, USHRT_MAX);
}

Test(read_progressbar, valid_cur_max_short)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[65535/3] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, USHRT_MAX);
    cr_assert_eq(bar.max, 3);
}

Test(read_progressbar, valid_max_max_short)
{
    struct progressbar_t bar;
    cr_assert_eq(read_progressbar("[3/65535] ", &bar, NULL), 0);
    cr_assert_eq(bar.cur, 3);
    cr_assert_eq(bar.max, USHRT_MAX);
}