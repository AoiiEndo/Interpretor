CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fsanitize=address
SRCS = main.c lexer.c parser.c
OBJS = $(SRCS:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

distclean: clean

.PHONY: all clean distclean
