CPPFLAGS = -I../include
CXXFLAGS = -O2 -g -Wall -fmessage-length=0
LDFLAGS  = -lpthread

SRCS = main.cpp queue.cpp thread_pool.cpp work_pool.cpp
OBJS = $(patsubst %cpp, %o, $(SRCS))
LIBS =
TARGET = thread_pool

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(LDFLAGS)

all: $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
