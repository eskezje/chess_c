# Program name
TARGET = chess
TEST_TARGET = tests

# Source Files
SRC = main.c board.c pieces.c
TEST_SRC = tests.c board.c pieces.c

# Take every .c file in SRC and replace .c with .o.
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET)


.PHONY: all test clean