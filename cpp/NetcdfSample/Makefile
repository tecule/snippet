CPPFLAGS = -I/opt/netcdf-cxx4/include
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
LDFLAGS  = -L/opt/netcdf-cxx4/lib -lnetcdf_c++4

SRCS = spei.cpp
OBJS = $(patsubst %cpp, %o, $(SRCS))

LIBS =

TARGET = spei

all: $(TARGET)

spei: spei.o
	$(CXX) -o spei spei.o $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<
	
clean:
	rm -f $(OBJS) $(TARGET)
