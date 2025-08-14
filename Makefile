# Program name
TARGET = chess

# Source Files
SRC = main.c board.c

# Take every .c file in SRC and replace .c with .o.
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -O2

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)