#ifndef MOTION_VECTOR_H
#define MOTION_VECTOR_H

#include "position.h"
#include <stdlib.h>

typedef struct {
  Position** macro_blocks;
  int rows, cols;
} MotionVector;


MotionVector create_motion_blocks(int rows, int cols);
void print_vector(MotionVector mv);
#endif
