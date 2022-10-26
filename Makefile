CFLAGS = -g3 -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align -g
CFLAGS += -Winline -Wfloat-equal -Wnested-externs
CFLAGS += -pedantic -std=gnu99 -Werror
CC = gcc
EXECS = 33sh 33noprompt

.PHONY: all clean

PROMPT = -DPROMPT

all: $(EXECS)

33sh: sh.c
	$(CC) $(CFLAGS) sh.c -o $@ -DPROMPT
33noprompt: sh.c
	$(CC) $(CFLAGS) sh.c -o $@
clean:
	rm -f $(EXECS)

