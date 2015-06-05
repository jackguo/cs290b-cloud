CC = gcc
CPP = g++

OBJS = isomorph.o jrb.o jval.o

bank: bank.cpp sqdb.cpp $(OBJS)
	$(CPP) $^ -l sqlite3 -lm -std=c++11 -o $@

$(OBJS) : isomorph.c jrb.c jval.c
	$(CC) $^ -lm -c

clean:
	rm *.o bank -rf
