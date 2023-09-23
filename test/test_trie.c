#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/trie.h"

START_TEST(test_trie_create)
{
	ck_assert_ptr_ne(trie_create(), NULL);
}

END_TEST START_TEST(test_trie_insert)
{
	trie_t *test_trie = trie_create();
	char target[] = "test";
	ck_assert_int_eq(trie_insert(NULL, NULL), 0);
	ck_assert_int_eq(trie_insert(&test_trie, NULL), 0);
	ck_assert_int_eq(trie_insert(NULL, target), 0);
	trie_destroy(&test_trie);
} END_TEST static TFun trie_tests[] = {

	test_trie_create,
	test_trie_insert,
	NULL
};

Suite *test_trie(void)
{
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
