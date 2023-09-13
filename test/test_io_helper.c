#include <check.h>
#include <stdio.h>
#include <stdint.h>

#include "io_helper.h"

START_TEST(test_validate_file_invalid) {
        const char *invalid_files[] = {
                "/dev/null",
                "/dev/urandom",
                "/dev/zero",
                "./test/",
                "./test/test_data/0_file"  // Manually created 0-size file
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

START_TEST (test_get_num_entries_valid) {
        FILE *fp = fopen("./test/test_data/data_4_entries.txt", "r");
        ck_assert_int_eq(get_num_entries(fp), 4);
}END_TEST

START_TEST(test_get_num_entries_invalid){
        ck_assert_int_eq(get_num_entries(NULL), 0);

}END_TEST

static TFun io_tests[] = {
        test_validate_file_invalid,
        test_validate_file_valid,
        test_get_num_entries_valid,
        test_get_num_entries_invalid,
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