CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

1019cc: $(OBJS)
			$(CC) -o 1019cc $(OBJS) $(LDFLAGS)

$(OBJS): 1019cc.h

test: 1019cc
	./test.sh

clean:
	rm -f 1019cc *.o *~ tmp* core* a.out

.PHONY: test clean