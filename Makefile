
###############################################################################
# Needed include dir
INCLUDE = $(PWD)/include
SRC = $(PWD)/src
CC = gcc
CCFLAGS = -Wall -I $(INCLUDE) -O3
LDFLAGS = -O3
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = motion-vector

# Go into src/ and run target motion-vector
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

###############################################################################

clean:
	rm -f *.o $(TARGET)
	rm -f $(SRC)/*.o
