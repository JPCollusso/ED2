CC = gcc
CFLAGS = -Wall -std=c99 -O2
TARGET = pgm

SRCS = pgm_handler.c pgm_ops.c main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

.INTERMEDIATE: $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)