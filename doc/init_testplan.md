# Initial Testplan

Based on design, there will be 4 compilation units to test:
1. io_helper.c
2. llist.c
3. hashtable.c
4. print_helper.c

## io_helper
io_helper is a pre-written library and tests will conform to previously written tests to validate files to ensure they are regular files of size greater than 0.

## llist
llist is also pulled from previously written library. The tests have been mostly written, and will center around creating, queue, dequeue, and is_empty, with a known data set to test llist capacity, as well as pointers returned from dequeue.

## hashtable
hashtable is another previously written library, however there is little unit testing at this point. The library will be modified to handle void pointers to contain structs of type player_t and team_t. Tests here should focus on proper pointers returned from create, that functions do not segfault on NULL arguments, and manual testing that printing the contents of a hashtable match the desired output from known data set.

## print_helper
This will be a completely custom library with new custom functions. The first tests should center around validating arguments against null or other invalid parameters. This too will contain manual testing to verify output against output in the rubric. Note: if time permits, look into `dup2()` as a way of creating a string from `stdout` that can be used with check for `ck_assert_str_eq` to alleviate the manual testing.