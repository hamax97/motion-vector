#include "motion_vector.h"

MotionVector create_motion_blocks(int cols, int rows)
{
  MotionVector mv;
  mv.rows = rows;
  mv.cols = cols;
  mv.macro_blocks = malloc(rows * cols * sizeof(Position));
  return mv;
}
