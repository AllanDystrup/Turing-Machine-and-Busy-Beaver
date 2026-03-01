# TURING CPP PROJECT
#
# ----- Switch to Ubuntu for Linux build tools
# C:\Users\allan>		wsl -d Ubuntu -u allan
# allan@LAPTOP-6UIHQ2QE:/mnt/c/Users/allan$  cd CLionProjects/turing/CPP/T32
# allan@LAPTOP-6UIHQ2QE:/mnt/c/Users/allan/CLionProjects/turing/CPP/T32$
#
# -----------------------------------------------------------------


# [0] FILES ------------------------------------------------------------------
#
default: list


# [1] ----- CLEAN -----------------------------------------------------------
clean:
	-rm -f ./bin/*.o
	ls -al ./bin

realclean:
	#-rm -f ./bin/*
	ls -al ./bin


# [2] ----- BUILD ----------------------------------------------------------
CC = g++
CFLAGS = -g -DMAIN -DDEBUG
#CFLAGS = -DDEBUG -g
LIBS = -lm

#	----- Dependencies -----
TTDEP =	./bin/GENERAL.o ./bin/TURING.o ./bin/BEAVER.o 
BTDEP =	./bin/GENERAL.o ./bin/TURING.o ./bin/BEAVER.o ./bin/SLIST.o ./bin/RANDOM.o


#	----- Targets -----
all:	GENERAL SLIST RANDOM SORT TURING BEAVER TMTEST BBTEST
	ls -al ./bin	


GENERAL:	GENERAL.cpp GENERAL.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o
		# no main testdriver
SLIST:   SLIST.cpp SLIST.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o
		# no main testdriver
	
RANDOM:  RANDOM.cpp RANDOM.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o
	#	$(CC) ./bin/$@.o -Wall $(LIBS) -o ./bin/$@
SORT:	 SORT.cpp SORT.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o
	#	$(CC) ./bin/$@.o -Wall $(LIBS) -o ./bin/$@

TURING:  TURING.cpp TURING.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o	
BEAVER:  BEAVER.cpp BEAVER.hpp
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o

TMTEST:  TMTEST.cpp GENERAL TURING BEAVER
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o	
		$(CC) ./bin/$@.o $(TTDEP) -Wall $(LIBS) -o ./bin/$@
BBTEST:  BBTEST.cpp GENERAL TURING BEAVER SLIST RANDOM
		$(CC) $(CFLAGS) -c  $<  -o ./bin/$@.o	
		$(CC) ./bin/$@.o $(BTDEP) -Wall $(LIBS) -o ./bin/$@

# [3] ----- TEST --------------------------------------------------
test:	
	./$(TARGET)

list:
	ls -al . ./bin
	

# ===== END TURING.Makefile #======================================

