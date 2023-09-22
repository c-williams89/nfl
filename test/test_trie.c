#include <check.h>
#include <stdio.h>
#include <stdlib.h>




static TFun trie_tests[] {
        NULL
};

Suite *test_trie(void) {
        Suite *s = suite_create("test_trie");
        TFun *curr = NULL;
        TCase *tc_core = tcase_create("trie");

        curr = trie_tests;
        while (*curr) {
                tcase_add_test(tc_core, *curr++);
        }
        suite_add_tcase(s, tc_core);
        return s;
}