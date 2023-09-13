#include <check.h>
#include <stdio.h>

#include "../include/hashtable.h"

static TFun hashtable_tests[] = {
        NULL
};

Suite *test_hashtable(void) {
        Suite *s = suite_create("test_hashtable");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("hashtable");

        curr = hashtable_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }

        suite_add_tcase(s, tc_core);
        return s;
}