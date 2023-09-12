#include <check.h>
#include <stdio.h>

#include "../include/io_helper.h"

START_TEST(test_validate_file_invalid) {
        const char *invalid_files[] = {
                "/dev/null",
                "/dev/urandom",
                "/dev/zero",
                "./test/",
                "./nfl_check"
        };

        for (int i = 0; i < 5; ++i) {
                FILE *fp = fopen(invalid_files[i], "r");
                ck_assert_int_eq(validate_file(fp), 0);
        }
}END_TEST

START_TEST(test_validate_file_valid) {
        FILE *fp = fopen("./test/test_data/nfldata.txt", "r");
        ck_assert_int_eq(validate_file(fp), 1);
}END_TEST

static TFun io_tests[] = {
        test_validate_file_invalid,
        test_validate_file_valid,
        NULL
};

Suite *test_io_helper(void) {
        Suite *s = suite_create("test_io_helper");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("io");

        curr = io_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }
        suite_add_tcase(s, tc_core);
        return s;
}