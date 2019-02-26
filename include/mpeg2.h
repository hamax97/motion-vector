#ifndef MPEG2_H
#define MPEG2_H

#include "position.h"
#include "motion_vector.h"
#include "bmp.h"
#include "macro_block.h"

MotionVector calc_motion_vector(BMP frame1, BMP frame2);

#endif
