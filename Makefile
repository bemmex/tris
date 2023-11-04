# the compiler: gcc for C program, define as g++ for C++
CC	 = gcc

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
FLAGS = -g -Wall -O2

LFLAGS = -lncurses
DESTINATION_FOLDER = obj
TARGET = $(DESTINATION_FOLDER)/tris

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ $(LFLAGS) 
#	$(RM) -r $(DESTINATION_FOLDER)/*.o
obj/%.o: src/%.c
	$(CC) $(FLAGS)  -c $< -o $@

clean:
	$(RM) -r $(DESTINATION_FOLDER)/*

install: 
	cp $(TARGET) /usr/local/bin

run:
	$(TARGET)