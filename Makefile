
###############################################################################
# Needed include dir
INCLUDE = $(PWD)/include
SRC = $(PWD)/src

# Go into src/ and run target motion-vector
all: motion-vector

motion-vector: motion-vector.o
	gcc -o $@ $<

motion-vector.o:
	gcc -c $(SRC)/motion-vector.c -I $(INCLUDE)

###############################################################################

clean:
	rm -rf motion-vector motion-vector.o
