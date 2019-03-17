#include <motion_vector.h>
#include <stdio.h>

MotionVector
create_motion_blocks(int rows, int cols)
{
  MotionVector mv;
  mv.rows = rows;
  mv.cols = cols;
  mv.macro_blocks = malloc(rows * sizeof(Position *));
  for(int i = 0; i < rows; i++)
    {
      mv.macro_blocks[i] = malloc(cols * sizeof(Position));
    }

  return mv;
}

void
print_positions(Position* macro_blocks, int rows, int cols)
{
  printf("[\n");
  for(int i = 0; i < rows; ++i) {
    for(int j = 0; j < cols; ++j) {
      printf("(%d, %d)  ",
	     macro_blocks[(i*rows) + j].y,
	     macro_blocks[(i*rows) + j].x);
    }
    printf("\n");
  }
  printf("]\n");
}

void
print_vector(MotionVector mv)
{
  printf("[ ");
  for(int i = 0; i < mv.rows; i++) {
    for(int j = 0; j < mv.cols; j++) {
      printf("(%03d, %03d)  ", mv.macro_blocks[i][j].y, mv.macro_blocks[i][j].x);
    }
    printf("\n");
    printf("  ");
  }
  printf("]");
}
