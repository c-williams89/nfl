#include <check.h>

extern Suite *test_io_helper(void);

int main (void) {
        SRunner *sr = srunner_create(NULL);
        
        Suite *test_io_helper_suite = test_io_helper();

        srunner_add_suite(sr, test_io_helper_suite);

#ifdef VALGRIND
        srunner_set_fork_status(sr, CK_NOFORK);
#endif
        srunner_run_all(sr, CK_VERBOSE);
        srunner_free(sr);
}