#include <check.h>

#include "../include/player.h"

START_TEST(test_player_create_invalid) {
        ck_assert_int_eq(player_create(NULL), 0);
}END_TEST

START_TEST(test_player_create_valid) {
        ck_assert_int_eq(player_create("abc"), 1);
}END_TEST

static TFun player_tests[] = {
        test_player_create_invalid,
        test_player_create_valid,
        NULL
};

Suite *test_player(void) {
        Suite *s = suite_create("test_player");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("player");

        curr = player_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }

        suite_add_tcase(s, tc_core);
        return s;
}