CXX=g++
CXXFLAGS=-g -Wall -Werror -O2 -std=c++23 -lcrypto
LIB_SRCS=$(wildcard encdec/src/*.cpp)
LIB_OBJS=$(LIB_SRCS:.cpp=.o)


all: $(LIB_OBJS) server.elf client.elf 


$(LIB_OBJS): $(LIB_SRCS) $(wildcard encdec/hdr/*.h)
	cd encdec/ && $(MAKE)
	cd ..

connection.o: src/connection.cpp hdr/connection.h
	$(CXX) $(CXXFLAGS) -c $<

send.o: src/send.cpp hdr/send.h hdr/version_defs.h $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -c $< $(LIB_OBJS)

%.elf: src/%.cpp connection.o send.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: tests/test.cpp connection.o send.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^


.PHONY: clean remake

clean:
	rm -f *.o test server.elf client.elf
	cd encdec/ && $(MAKE) clean
	cd ..

remake: clean all
