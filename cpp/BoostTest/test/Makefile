TARGET = test_my_add
SRCS = test_my_add.cpp ../src/my_add.c
OBJS = test_my_add.o ../src/my_add.o

CPPFLAGS = -I../include -I/opt/boost/include
CXXFLAGS = -g -Wall
# LDFLAGS = -L/opt/boost/lib -Wl,-Bstatic -lboost_unit_test_framework
LDFLAGS = -L/opt/boost/lib -lboost_unit_test_framework -Wl,-rpath,/opt/boost/lib/

all: $(TARGET)

.PHONY: clean

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)
	
%.o:%.cpp
	g++ -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<
	
clean:
	rm -rf $(TARGET) test_my_add.o