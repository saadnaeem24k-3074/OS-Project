

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2
LIBS    = -lraylib -lpthread -lm -ldl

# All three .c source files compiled and linked together
SRCS    = traffic.c simulation.c main.c
TARGET  = traffic_raylib

all: $(TARGET)

$(TARGET): $(SRCS) traffic.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
