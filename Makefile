CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = cdotenv
SRCS    = cdotenv.c main.c cdotenv_extended.c 
OBJS    = $(SRCS:.c=.o)
HDRS    = cdotenv.h cdotenv_extended.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
