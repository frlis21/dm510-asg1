#include <errno.h>
#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long _msgbox_put(char *buf, size_t len) {
	long ret = syscall(__NR_dm510_msgbox_put, buf, len);

	if (ret)
		perror("\tmsgbox_put");

	return ret;
}

long _msgbox_get(char *buf, size_t len) {
	int mlen = syscall(__NR_dm510_msgbox_get, buf, len);

	if (mlen < 0)
		perror("\tmsgbox_get");
	else {
		fprintf(stderr, "\tGot %d bytes:\n", mlen);
		fprintf(stderr, "\t%.*s\n", mlen, buf);
	}

	return mlen;
}

static unsigned long test_num = 0;

#define msgbox_put(buf, len)                                                   \
	{                                                                          \
		fprintf(stderr, "%lu. msgbox_put(" #buf ", " #len "):\n", ++test_num); \
		_msgbox_put(buf, len);                                                 \
	}

#define msgbox_get(buf, len)                                                   \
	{                                                                          \
		fprintf(stderr, "%lu. msgbox_get(" #buf ", " #len "):\n", ++test_num); \
		_msgbox_get(buf, len);                                                 \
	}

void msg(const char *message) { fprintf(stderr, "%s\n", message); }

void sep() {
	fprintf(stderr, "----------------------------------------"
	                "----------------------------------------\n");
}

#define BUF_LEN 42
#define DUMMY_DATA "dummy message"
#define DUMMY_DATA_LEN 13
#define DUMMY_N 20

int main(int argc, char *argv[]) {
	char buffer[BUF_LEN];

	// Clear the msgbox
	while (syscall(__NR_dm510_msgbox_get, buffer, BUF_LEN) >= 0)
		;

	msg("(cleared msgbox)");
	sep();

	msg("msgbox_get from empty msgbox?");
	msgbox_get(buffer, BUF_LEN);
	sep();

	// Fill the msgbox
	for (int i = 0; i < DUMMY_N; i++)
		syscall(__NR_dm510_msgbox_put, DUMMY_DATA, DUMMY_DATA_LEN);

	fprintf(stderr, "(filled msgbox with %dx%d bytes \"%s\")\n", DUMMY_N,
	        DUMMY_DATA_LEN, DUMMY_DATA);
	sep();

	msg("Nulls and stuff?");
	msgbox_put(NULL, 42);
	msgbox_get(NULL, 42);
	sep();

	msg("Invalid addresses?");
	msgbox_put((char *)-42, 42);
	msgbox_put((char *)42, 42);
	msgbox_get((char *)-42, 42);
	msgbox_get((char *)42, 42);
	sep();

	msg("Negative numbers?");
	msgbox_put(buffer, -42);
	msgbox_get(buffer, -42);
	sep();

	msg("And everything should still work after all of that!");
	msgbox_get(buffer, BUF_LEN);
	msgbox_put("hello", 5);
	msgbox_get(buffer, BUF_LEN);

	return 0;
}
