floyd_SRC = floyd.c util.c
floyd_OBJ = $(patsubst %.c, %.o, $(floyd_SRC))
PROGRAM = floyd

CPPFLAG = -I../include -I/opt/openmpi/include
CFLAGS = -g -Wall -DNDEBUG

.PHONY: clean

all: $(PROGRAM)

$(PROGRAM): $(floyd_OBJ)
	mpicc -o $(PROGRAM) $(CFLAGS) $(floyd_OBJ)
%.o: %.c
	mpicc -c -o $@ $(CPPFLAG) $(CFLAGS) $<

clean:
	rm -f $(PROGRAM) $(floyd_OBJ)
