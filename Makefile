# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic


# Source files for standalone C program
C_SRCS = *.cpp
C_OBJS = $(C_SRCS:.cpp=.o)
C_TARGET = myrobot

all: $(C_TARGET)

$(C_TARGET): $(C_OBJS)
	$(CC) $(CFLAGS) -o $(C_TARGET) $(C_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Clean up
clean:
	rm -f $(C_OBJS) $(C_TARGET)

# Phony targets
.PHONY: all $(C_TARGET) clean