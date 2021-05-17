

TARGET= executor.out
CC=gcc
CFLAGS= -g  -pedantic -Wall  -Iinclude
OBJS= test.o  executor.o  task.o clock.o heap.o vector.o

INCLUDE_PATH= include/
SRC_PATH= src/
TEST_PATH= test/

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
test.o: $(TEST_PATH)test.c $(INCLUDE_PATH)executor.h $(INCLUDE_PATH)misc.h 
	$(CC) $(CFLAGS) -c $(TEST_PATH)test.c	
clock.o: $(SRC_PATH)clock.c $(INCLUDE_PATH)clock.h $(INCLUDE_PATH)misc.h 
	$(CC) $(CFLAGS) -c $(SRC_PATH)clock.c

task.o: $(SRC_PATH)task.c  $(INCLUDE_PATH)task.h $(INCLUDE_PATH)misc.h $(INCLUDE_PATH)clock.h
	$(CC) $(CFLAGS) -c $(SRC_PATH)task.c

executor.o: $(SRC_PATH)executor.c $(INCLUDE_PATH)executor.h  $(INCLUDE_PATH)task.h $(INCLUDE_PATH)misc.h $(INCLUDE_PATH)clock.h $(INCLUDE_PATH)heap.h
	$(CC) $(CFLAGS) -c $(SRC_PATH)executor.c
heap.o: $(SRC_PATH)heap.c  $(INCLUDE_PATH)heap.h $(INCLUDE_PATH)vector.h
	$(CC) $(CFLAGS) -c $(SRC_PATH)heap.c

vector.o: $(SRC_PATH)vector.c  $(INCLUDE_PATH)vector.h 
	$(CC) $(CFLAGS) -c $(SRC_PATH)vector.c

test: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(TARGET) $(OBJS)