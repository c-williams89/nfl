#include <check.h>
#include <stdio.h>
#include <stdlib.h>



TFun llist_tests[] = {
        NULL
};

Suite *test_llist(void) {
        Suite *s = suite_create("test_llist");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("llist");

        curr = llist_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }
        
        suite_add_tcase(s, tc_core);
        return s;
}