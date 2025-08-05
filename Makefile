CXX=g++
CXXFLAGS=-g -Wall -Werror -O2


all: server.elf client.elf

connection.o: src/connection.cpp hdr/connection.h
	$(CXX) $(CXXFLAGS) -c $^

%.elf: src/%.cpp connection.o
	$(CXX) $(CXXFLAGS) -o $@ $^



.PHONY: clean

clean:
	rm *.o hdr/*.gch
