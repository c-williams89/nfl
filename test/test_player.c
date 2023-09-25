#include <check.h>

#include "../include/player.h"
#include "../include/hashtable.h"

uint64_t test_hash(char *key)
{
	uint64_t hash = 5381;
	int c;

	while ((c = *key++)) {
		hash = ((hash << 5) + hash) + c;

	}
	return hash;
}

START_TEST(test_player_create_invalid)
{
	hash_t *test_table = hash_table_create(1, test_hash);
	char test[] = "Test";
	ck_assert_ptr_eq(player_create(NULL, NULL), NULL);
	ck_assert_ptr_eq(player_create(test_table, NULL), NULL);
	ck_assert_ptr_eq(player_create(NULL, test), NULL);
}

END_TEST START_TEST(test_player_create_valid)
{
	hash_t *test_table = hash_table_create(1, test_hash);
	char test[] = "Test";
	ck_assert_ptr_ne(player_create(test_table, test), NULL);
}

END_TEST START_TEST(test_player_insert)
{
	hash_t *test_table = hash_table_create(1, test_hash);
	char test[] = "test";
	player_t *test_player = player_create(test_table, test);
	ck_assert_int_eq(player_insert(NULL, NULL), 0);
	ck_assert_int_eq(player_insert(test_player, NULL), 0);
	ck_assert_int_eq(player_insert(NULL, test_table), 0);
} END_TEST static TFun player_tests[] = {

	test_player_create_invalid,
	test_player_create_valid,
	test_player_insert,
	NULL
};

Suite *test_player(void)
{
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
