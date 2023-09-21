#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/llist.h"

START_TEST(test_llist_create) {
        llist_t *llist = llist_create();
        ck_assert_ptr_ne(llist, NULL);
}END_TEST

START_TEST(test_llist_enqueue_valid) {
        llist_t *llist = llist_create();
        int x = 0;
        void *data = &x;
        ck_assert_int_eq(llist_enqueue(llist, data), 1);
}END_TEST

START_TEST(test_llist_enqueue_invalid) {
        ck_assert_int_eq(llist_enqueue(NULL, NULL), 0);
        int x = 0;
        void *data = &x;
        ck_assert_int_eq(llist_enqueue(NULL, data), 0);
        llist_t *llist = llist_create();
        ck_assert_int_eq(llist_enqueue(llist, NULL), 0);

}END_TEST

TFun llist_tests[] = {
        test_llist_create,
        test_llist_enqueue_valid,
        test_llist_enqueue_invalid,
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