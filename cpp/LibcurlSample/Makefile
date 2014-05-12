libcurl_demo_SOURCES = libcurl_demo.c
libcurl_demo_OBJECTS = $(patsubst %c, %o, $(libcurl_demo_SOURCES))
PROGRAMS = libcurl_demo

CPPFLAGS = 
CFLAGS = -g -Wall
LDFLAGS = -lcurl

all: $(PROGRAMS)

$(PROGRAMS): $(libcurl_demo_OBJECTS)
	gcc $(CFLAGS) -o $(PROGRAMS) $(libcurl_demo_OBJECTS) $(LDFLAGS) 

%.o: %.c
	gcc -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

.PHONY: clean	
clean:
	rm -f $(PROGRAMS) $(libcurl_demo_OBJECTS)

