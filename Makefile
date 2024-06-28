# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic


# Source files for standalone C program
C_SRCS = $(wildcard *.cpp)
C_TARGET = myrobot

all: $(C_TARGET)

$(C_TARGET): $(C_OBJS)
	$(CC) $(CFLAGS) -o $(C_TARGET) $(C_SRCS)


# Clean up
clean:
	rm -f $(C_OBJS) $(C_TARGET)

# Phony targets
.PHONY: all $(C_TARGET) clean