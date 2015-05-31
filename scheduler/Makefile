CC = g++
LIBS = -l sqlite3

OBJS = GraphStore.o

all: $(OBJS)

GraphStore.o: GraphStore.cpp
	$(CC) $^ $(LIBS) -c

test_gs: GraphStore.cpp TestGraphStore.cpp
	$(CC) $^ $(LIBS) -o $@

clean:
	rm $(OBJS) test_gs -f
