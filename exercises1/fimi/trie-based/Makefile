SHELL = /bin/sh
CXX = g++
CPPFLAGS = -O3 -Wall

OBJECTS = ./Input_Output_Manager.o ./Apriori.o Trie.o ./Apriori_Trie.o ./main.o
CPPS = $(OBJECTS:.o=.cpp)
HPPS = $(subst .o,.hpp,$(subst main.o,,$(OBJECTS))) common.hpp


ifdef GPROF
	CPPFLAGS = -pg -fprofile-arcs -Wall
	LDFLAGS = -pg
endif

ifdef DEBUG
	CPPFLAGS = -g -fno-inline -Wall
endif


all: ../fim_all

../fim_all: $(OBJECTS)
	$(CXX) $^ -o ../fim_all $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

.PHONY  : doc zip bin clean
doc : 
	doxygen
zip : doc
	tar --directory=.. -cvzf ../apriori_bodon.tar.gz Documentation $(addprefix source/,$(CPPS)) $(addprefix source/,$(HPPS)) source/Doxyfile source/Makefile
bin : ../apriori 
	tar -cvzf ../apriori_linux.tar.gz ../apriori
clean:
	rm -f $(OBJECTS) ../fim_all