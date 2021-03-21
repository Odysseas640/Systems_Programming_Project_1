CC=g++
CFLAGS=-Wall -std=c++11

all: date.o date_tree.o hash_table.o patientRecord.o max_heap.o main.o diseaseMonitor

date.o: date.cpp date.h
	$(CC) $(CFLAGS) -c date.cpp

date_tree.o: date_tree.cpp date_tree.h
	$(CC) $(CFLAGS) -c date_tree.cpp

hash_table.o: hash_table.cpp hash_table.h
	$(CC) $(CFLAGS) -c hash_table.cpp

patientRecord.o: patientRecord.cpp patientRecord.h
	$(CC) $(CFLAGS) -c patientRecord.cpp

max_heap.o: max_heap.cpp max_heap.h
	$(CC) $(CFLAGS) -c max_heap.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

diseaseMonitor: main.cpp patientRecord.o hash_table.o date_tree.o max_heap.o date.o
	$(CC) $(CFLAGS) -o diseaseMonitor main.o hash_table.o patientRecord.o date_tree.o max_heap.o date.o

.PHONY: clean

clean:
	rm -f diseaseMonitor main.o hash_table.o patientRecord.o date_tree.o max_heap.o date.o
