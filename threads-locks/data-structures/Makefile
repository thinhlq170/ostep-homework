CC = gcc
CFLAGS = -Wall -Werror

TARGET = test-concurrent-table

objects = $(wildcard *.o)

all: $(TARGET)

$(TARGET): hash-type.o list-type.o counter-type.o approx-counter-type.o test-concurrent-table.o
	$(CC) $(CFLAGS) $^ -o $@

simple_counter: counter-type.o test_simple_counter.o
	$(CC) $(CFLAGS) $^ -o $@

hash-type.o: hash-type.c
	$(CC) $(CFLAGS) -c $^ -o $@

list-type.o: list-type.c
	$(CC) $(CFLAGS) -c $^ -o $@

counter-type.o: counter-type.c
	$(CC) $(CFLAGS) -c $^ -o $@
	
approx-counter-type.o: approx-counter-type.c
	$(CC) $(CFLAGS) -c $^ -o $@

test_simple_counter.o: test_simple_counter.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $^ -o $@



clean: 
	rm -f $(TARGET) $(wildcard *.o)