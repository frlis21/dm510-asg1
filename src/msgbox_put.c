#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "no\n");
		return 1;
	}

	if (syscall(__NR_dm510_msgbox_put, argv[1], strlen(argv[1]))) {
		perror(argv[0]);
		return 1;
	}

	return 0;
}
