add_SRCS = my_add.c
add_OBJS = $(patsubst %c, %o, $(add_SRCS))

CPPFLAGS = -I../include
CFLAGS = -g -Wall

all: $(add_OBJS)

%.o:%.c
	gcc -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(add_OBJS)
