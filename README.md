# nfl

## Description
`nfl` is a program that loads a player/team database of NFL players from 1960
through 2019 and calculates the degrees of separation between any two players,
as well as provide statistics for any given player or team through the use of
specified command line options.

## Installation
The entire projet can be cloned from Git.

Developed with C18 and compiled with gcc-9.

---

### Build
`nfl` can be built using the make tool.
To build and compile the program:
```sh
<project_root>$ make
```
to create the executable `nfl`.

---

### Run
`nfl` must be invoked with one and only one of the following required options:

```sh 
--player <PLAYER>
--search <SEARCH_PARAM>
--stats <PLAYER>
--distance <PLAYER> <PLAYER> [-i]
--teams
--roster <TEAM_NAME> <TEAM_YEAR>
--oracle
```
`nfl` can accept the following optional argument:
```sh
-f <FILE>
```
To specify a file other than the default player/team database included with the repo.

#### Option Flags
`--player <PLAYER>` - Accepts a player argument as either player name or ID, and
prints their id, name, position, college, and list of NFL teams they have played
for.
EX:
```sh
$ ./nfl --player "Andy Dalton"
DaltAn00	Andy Dalton	QB	TCU
	2011 - Cincinnati Bengals
	2012 - Cincinnati Bengals
	2013 - Cincinnati Bengals
	2014 - Cincinnati Bengals
	2015 - Cincinnati Bengals
	2016 - Cincinnati Bengals
	2017 - Cincinnati Bengals
	2018 - Cincinnati Bengals
	2019 - Cincinnati Bengals
```

`--search <SEARCH_PARM>` - Prints a list of all players (id, name, position, college,
and first NFL team) of any players who's name or college contains the search parameter.
EX:
```sh
$ ./nfl --search "Cornell"
BootKe20	Kevin Boothe	Cornell
ArmsCo00	Cornell Armstrong	Southern Miss
BrowCo20	Cornell Brown	Virginia Tech
PaynSe20	Seth Payne	Cornell
BurbCo00	Cornell Burbage	Kentucky
CornJa00	Jack Cornell	Illinois
LallBo20	Bob Lally	Cornell
LarsPe00	Pete Larson	Cornell
...
```

`--stats <PLAYER>` - Accepts a player argument as either player name or id, and 
calculates and prints number of players at each degree of separation, as well as
average separation.
EX:
```sh
$ ./nfl --stats "DaltAn00"
Network stats for Andy Dalton
0 -- 1 cohort
1 -- 214 cohorts
2 -- 6628 cohorts
3 -- 5958 cohorts
4 -- 4724 cohorts
5 -- 2777 cohorts
6 -- 188 cohorts
7 -- 0 cohorts
8 -- 0 cohorts
9 -- 0 cohorts
Average separation 3.184627
```

`--distance <PLAYER> <PLAYER>` - Accepts two players as name or id and prints the 
shortest connection between the two. The additional `-i <IGNROED_PLAYER>` option, 
if provided, will exclude that player from consideration.
**NOTE: Ignored players can not be either of the player argument to `--distance`.**
```sh
$ ./nfl --distance "Johnny Manziel" "Roger Staubach"
Roger Staubach played for Dallas Cowboys in 1974 with Too Tall Jones
Too Tall Jones played for Dallas Cowboys in 1989 with Troy Aikman
Troy Aikman played for Dallas Cowboys in 1998 with Flozell Adams
Flozell Adams played for Dallas Cowboys in 2006 with Miles Austin
Miles Austin played for Cleveland Browns in 2014 with Johnny Manziel

$ ./nfl --distance "Johnny Manziel" "Roger Staubach" -i "Troy Aikman"
Roger Staubach played for Dallas Cowboys in 1979 with Doug Cosbie
Doug Cosbie played for Dallas Cowboys in 1988 with Michael Irvin
Michael Irvin played for Dallas Cowboys in 1998 with Flozell Adams
Flozell Adams played for Dallas Cowboys in 2006 with Miles Austin
Miles Austin played for Cleveland Browns in 2014 with Johnny Manziel
```

`--teams` - Prints a list of unique teams.
```sh
$ ./nfl --teams
Arizona Cardinals
Atlanta Falcons
Baltimore Colts
...
```

`--roster <TEAM_NAME> <TEAM_YEAR>` - Prints a roster of the the team specified by
name and year.
EX:
```sh
$ ./nfl --roster "Dallas Cowboys" 1970
	AddeHe00	Herb Adderley	LCB	Michigan St.
	AdkiMa00	Margene Adkins		Henderson JC
	AndrGe00	George Andrie	RDE	Marquette
	AsheBo20	Bob Asher	RT	Vanderbilt
```

`--oracle` - Will calculate stats for every player in the database, and print the
player with the best and worst average separation.
EX:
```sh
$ ./nfl --oracle
And the winners are:
The center of the universe (since 1960) with a score of 2.289849 is Morten Andersen
The least connected NFL player with at least 100 connections of the NFL universe (since 1960) with a score of 4.207321 is Dewey Bohling
```

### Debug/Error Checking
To build `nfl` with debug flags:
```sh
<project_root> $ make debug
```
to be able to debug with gdb.
```sh
<project_root> $ gdb --args ./nfl <OPTION> <ARG> [-f] <FILE> [-i] <PLAYER>
```

To check `nfl` against valgrind for memory leaks and errors, build with debug 
flags and run with `valgrind` prepended.
```sh
<project_root> $ make debug
<project_root> $ valgrind ./nfl <OPTION> <ARG> [-f] <FILE> [-i] <PLAYER>
```

## Performance/Optimization
To be able to view performance information, build and execute with the following
profiling flags:
```sh 
<project_root> $ make profile
<project_root> $ ./nfl <OPTION> <ARG> [-f] <FILE> [-i] <PLAYER>
```
After the program has been run, use `gprof` to view graph profile.
```sh
<project_root> $ gprof nfl gmon.out > doc/analysis.txt
```
**NOTE: gprof will create gmon.out in the top level directory by default. However,
this command redirects the report to text format in `doc/`**

---

### Testing
Automated unit tests have been written for `nfl` and can be run using make.
```sh
<project_root> $ make check
```
will build and run automated unit testing.
```sh
100%: Checks: 14, Failures: 0, Errors: 0
test/test_hashtable.c:20:P:hashtable:*curr++:0: Passed
test/test_hashtable.c:26:P:hashtable:*curr++:0: Passed
test/test_hashtable.c:38:P:hashtable:*curr++:0: Passed
test/test_hashtable.c:51:P:hashtable:*curr++:0: Passed
test/test_llist.c:21:P:llist:*curr++:0: Passed
test/test_llist.c:29:P:llist:*curr++:0: Passed
test/test_llist.c:37:P:llist:*curr++:0: Passed
test/test_llist.c:45:P:llist:*curr++:0: Passed
test/test_llist.c:59:P:llist:*curr++:0: Passed
test/test_llist.c:73:P:llist:*curr++:0: Passed
test/test_llist.c:85:P:llist:*curr++:0: Passed
test/test_llist.c:89:P:llist:*curr++:0: Passed
test/test_trie.c:9:P:trie:*curr++:0: Passed
test/test_trie.c:18:P:trie:*curr++:0: Passed
```
Additional information about `nfl` testing can be found [here](./doc/testplan.pdf)

---

### Cleanup
After running the program, execute:
```sh
<project_root> $ make clean
```
to delete object files as well as data created from `make profile`.

---

## Support
Additional support can be found in the man page:
```sh
<project_root> $ man ./doc/nfl.1
```
