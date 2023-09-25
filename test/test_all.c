#include <check.h>

extern Suite *test_io_helper(void);
extern Suite *test_hashtable(void);
extern Suite *test_player(void);
extern Suite *test_llist(void);
extern Suite *test_trie(void);

int main(void)
{
	SRunner *sr = srunner_create(NULL);

	Suite *test_io_helper_suite = test_io_helper();
	Suite *test_hashtable_suite = test_hashtable();
	Suite *test_player_suite = test_player();
	Suite *test_llist_suite = test_llist();
	Suite *test_trie_suite = test_trie();

	srunner_add_suite(sr, test_io_helper_suite);
	srunner_add_suite(sr, test_hashtable_suite);
	srunner_add_suite(sr, test_player_suite);
	srunner_add_suite(sr, test_llist_suite);
	srunner_add_suite(sr, test_trie_suite);

#ifdef VALGRIND
	srunner_set_fork_status(sr, CK_NOFORK);
#endif
	srunner_run_all(sr, CK_VERBOSE);
	srunner_free(sr);
}
