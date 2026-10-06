CC = gcc
CFLAGS = -Wall -Wextra

TARGET = minimat

OBJS = main.o struct.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c struct.h
	$(CC) $(CFLAGS) -c main.c

struct.o: struct.c struct.h
	$(CC) $(CFLAGS) -c struct.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)