#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/llist.h"

llist_t *populate_list(int elements) {
        llist_t *llist = llist_create();
        for (int i = 0; i < elements; ++ i) {
                int *x = calloc(1, sizeof(int));
                *x = i;
                llist_enqueue(llist, x);
        }
        return llist;
}

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

START_TEST(test_llist_dequeue_valid) {
        int num_elements = 20;
        llist_t *llist = populate_list(num_elements);
        for (int i = 0; i < num_elements; ++i) {
                int *x = (int *)llist_dequeue(llist);
                ck_assert_int_eq(*x, i);
        }

}END_TEST

START_TEST(test_llist_dequeue_invalid) {
        llist_t *llist = NULL;
        ck_assert_ptr_eq(llist_dequeue(llist), NULL);
        llist = llist_create();
        ck_assert_ptr_eq(llist_dequeue(llist), NULL);
        int num_elements = 10;
        llist = populate_list(num_elements);
        for (int i = 0; i < num_elements; ++i) {
                llist_dequeue(llist);
        }
        ck_assert_ptr_eq(llist_dequeue(llist), NULL);

}END_TEST

START_TEST(test_llist_is_empty) {
        llist_t *llist = NULL;
        ck_assert_int_eq(llist_is_empty(llist), 0);
        int num_elements = 10;
        llist = llist_create();
        ck_assert_int_eq(llist_is_empty(llist), 1);
        llist = populate_list(num_elements);
        ck_assert_int_eq(llist_is_empty(llist), 0);
        for (int i = 0; i < num_elements; ++i) {
                llist_dequeue(llist);
        }
        ck_assert_int_eq(llist_is_empty(llist), 1);
}END_TEST

TFun llist_tests[] = {
        test_llist_create,
        test_llist_enqueue_valid,
        test_llist_enqueue_invalid,
        test_llist_dequeue_valid,
        test_llist_dequeue_invalid,
        test_llist_is_empty,
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