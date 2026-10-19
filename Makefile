CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = cdotenv
SRCS    = main.c cdotenv.c cdotenv_utils.c
OBJS    = $(patsubst %.c,.%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.%.o: %.c cdotenv.h cdotenv_utils.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
