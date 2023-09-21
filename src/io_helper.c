#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

enum { MAX_LINE = 512 };

int validate_file(FILE * fp)
{
	int exit_status = 0;
	// if (!fp) {
	//         fprintf(stderr, "validate_file: Invalid file pointer - NULL\n");
	//         goto EXIT;
	// }

	struct stat path_stat;
	int fd = fileno(fp);
	if (-1 == fstat(fd, &path_stat)) {
		perror("validate_file");
		errno = 0;
		goto EXIT;
	}

	if (S_ISREG(path_stat.st_mode)) {
		if (0 < path_stat.st_size) {
			exit_status = 1;
		}
	}

 EXIT:
	return exit_status;
}

uint16_t get_num_entries(FILE * fp)
{
	uint16_t num_entries = 0;
	if (!fp) {
		fprintf(stderr,
			"get_num_entries: Invalid file pointer - NULL\n");
		goto EXIT;
	}

	char *buf = NULL;
	size_t len = 0;

	while (getline(&buf, &len, fp) != -1) {
		++num_entries;
	}
	free(buf);

	rewind(fp);
 EXIT:
	return num_entries;
}
