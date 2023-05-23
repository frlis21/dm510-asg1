#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "no\n");
		return 1;
	}

	char *endptr;
	int length = strtol(argv[1], &endptr, 10);

	if (length < 0 || argv[1] == endptr) {
		fprintf(stderr, "stop\n");
		return 1;
	}

	char *buffer = malloc(length);
	length = syscall(__NR_dm510_msgbox_get, buffer, length);

	if (length < 0) {
		perror(argv[0]);
		return 1;
	}

	printf("msgbox_get got %d bytes:\n%.*s\n", length, length, buffer);
	free(buffer);

	return 0;
}
