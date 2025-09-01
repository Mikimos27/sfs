CXX=g++
CXXFLAGS=-g -Wall -Werror -O2


all: server.elf client.elf 


connection.o: src/connection.cpp hdr/connection.h
	$(CXX) $(CXXFLAGS) -c $<

send.o: src/send.cpp hdr/send.h hdr/version_defs.h
	$(CXX) $(CXXFLAGS) -c $<

%.elf: src/%.cpp connection.o send.o
	$(CXX) $(CXXFLAGS) -o $@ $^



.PHONY: clean

clean:
	rm *.o hdr/*.gch
