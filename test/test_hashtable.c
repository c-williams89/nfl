#include <check.h>
#include <stdio.h>

#include "../include/hashtable.h"

uint64_t hash(char *key)
{
	uint64_t hash = 5381;
	int c;

	while ((c = *key++)) {
		hash = ((hash << 5) + hash) + c;

	}
	return hash;
}

START_TEST(test_hash_table_create_valid)
{
	ck_assert_ptr_ne(hash_table_create(32, hash), NULL);
}

END_TEST START_TEST(test_hash_table_create_invalid)
{
	ck_assert_ptr_eq(hash_table_create(0, hash), 0);
	ck_assert_ptr_eq(hash_table_create(32, NULL), 0);
}

END_TEST 

START_TEST(test_hash_table_insert) {
	hash_t *test_table = hash_table_create(32, hash);
	char key[] = "hashtable_insert_test";
	int x = 42;
	void *data = &x;
	ck_assert_int_eq(hash_table_insert(test_table, NULL, NULL), 0);
	ck_assert_int_eq(hash_table_insert(NULL, key, NULL), 0);
	ck_assert_int_eq(hash_table_insert(test_table, NULL, data), 0);
	ck_assert_int_eq(hash_table_insert(test_table, key, data), 1);
}END_TEST

static TFun hashtable_tests[] = {
	test_hash_table_create_valid,
	test_hash_table_create_invalid,
	test_hash_table_insert,
	NULL
};

Suite *test_hashtable(void)
{
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
