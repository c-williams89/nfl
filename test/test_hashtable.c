#include <check.h>
#include <stdio.h>

#include "../include/hashtable.h"

uint64_t hash(char *key) {
        uint64_t hash = 5381;
        int c;
        c = *key++;

        while (key) {
                hash = ((hash << 5) + hash) + c;
                c = *key++;
        }
        return hash;
}

START_TEST(test_hash_table_create_valid) {
        ck_assert_int_eq(hash_table_create(32, hash), 1);
}END_TEST

START_TEST (test_hash_table_create_invalid) {
        ck_assert_int_eq(hash_table_create(NULL, hash), 0);
}END_TEST

static TFun hashtable_tests[] = {
        test_hash_table_create_valid,
        test_hash_table_create_invalid,
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