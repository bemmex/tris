# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -O2

# external library
LDFLAGS = -lncurses

# the build target executable:
TARGET = tris

DESTINATION_FOLDER = ../obj/
OTHER = core.c layout.c widget.c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(DESTINATION_FOLDER)$(TARGET) $(TARGET).c $(OTHER) $(LDFLAGS)

clean:
	$(RM) $(DESTINATION_FOLDER)$(TARGET)

run:
	$(DESTINATION_FOLDER)$(TARGET)