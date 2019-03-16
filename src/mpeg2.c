#include <mpeg2.h>
#include <omp.h>
#include <limits.h>

MacroBlock fill_macro_block(BMP frame, int x, int y);
Position search_macro_block(BMP frame, MacroBlock *mb);

int evaluate(BMP frame, MacroBlock* mb, int posy, int posx,
	 Position* min_pos, int min_value);

int movey(BMP frame, MacroBlock* mb, int posy,
	  Position* min_pos, int min_value,
	  int limit_left, int limit_right);

int movex(BMP frame, MacroBlock* mb, int posx,
	   Position* min_pos, int min_value,
	  int limit_up, int limit_down);

extern int rank;

MotionVector calc_motion_vector(BMP frame1, BMP frame2) {
  int num_blocks_y = frame1.height / 16;
  int num_blocks_x = frame1.width / 16;
  MotionVector mv = create_motion_blocks(num_blocks_y, num_blocks_x);
  int size = num_blocks_y * num_blocks_x;

  printf("%d FRAME1 HEIGHT %d HEIGHT/16 %d\n", rank, frame1.height, frame1.height);

  printf("%d Frame1 size %d x %d\n", rank, frame1.height, frame1.width);
  printf("%d Frame2 size %d x %d\n", rank, frame2.height, frame2.width);
  printf("%d Created motion vector\n", rank);

#pragma omp parallel
  {
#pragma omp for
    for(int i = 0; i < size; i++) {
      int posy = i / num_blocks_x;
      int posx = i % num_blocks_x;
      MacroBlock mb = fill_macro_block(frame1, 16*posy, 16*posx);
      Position new_pos = search_macro_block(frame2, &mb);
      mv.macro_blocks[posy][posx] = new_pos;
      //printf("macroblock: (%i, %i) new pos: (%i, %i)\n", j, i, new_pos.y, new_pos.x);
    }
  }

  printf("Process %d going out from calc_motion_vector\n", rank);
  return mv;
}


MacroBlock fill_macro_block(BMP frame, int y, int x) {
  //printf("%d Started fill\n", rank);
  MacroBlock mb;
  mb.x = x;
  mb.y = y;

  for(int i = 0; i < 16; i++)
    {
      int posy = y + (i*frame.width);
      for(int j = 0; j <16; j++)
	{
	  int posx = x + j;
	  mb.block[i][j] = frame.pixels[posy + posx];
	}
    }
  //printf("%d Finished fill\n", rank);
  return mb;
}


Position
search_macro_block(BMP frame, MacroBlock *mb) {
  int last_blocky = frame.height - 16;
  int last_blockx = frame.width - 16;
  Position min_pos;
  int min_value = INT_MAX;

  int up, down;
  up = down = mb->y;
  int left, right;
  left = right = mb->x;
  char can_up, can_down, can_left, can_right, can_move;
  can_up = can_down = can_left = can_right = can_move = 0;

  if(evaluate(frame, mb, mb->y, mb->x, &min_pos, min_value) == 0) return min_pos;

  if(up != 0){
      can_up = 1;
      can_move++;
      up--;
  }
  if(down < last_blocky-1) {
    can_down = 1;
    can_move++;
    down++;
  }
  if(left != 0) {
    can_left = 1;
    can_move++;
    left--;
  }
  if(right < last_blockx-1) {
    can_right = 1;
    can_move++;
    right++;
  }

  while(can_move){

    if(can_up) {
      min_value = movey(frame, mb, up, &min_pos, min_value, left, right);
      if(min_value == 0) break;
      if(up - 1 < 0) {
	can_up = 0;
	can_move--;
      } else up--;
    }

    if(can_down) {
      min_value = movey(frame, mb, down, &min_pos, min_value, left, right);
      if(min_value == 0) break;
      if(down + 1 >= last_blocky) {
	can_down = 0;
	can_move--;
      } else down++;
    }

    if(can_left) {
      min_value = movex(frame, mb, left, &min_pos, min_value, up + can_up, down - can_down);
      if(min_value == 0) break;
      if(left - 1 < 0) {
	can_left = 0;
	can_move--;
      }  else left--;
    }

    if(can_right) {
      min_value = movex(frame, mb, right, &min_pos, min_value, up + can_up, down - can_down);
      if(min_value == 0) break;
      if(right + 1 >= last_blockx) {
	can_right = 0;
	can_move--;
      } else right++;
    }
  }
  return min_pos;
}

int
evaluate(BMP frame, MacroBlock* mb, int posy, int posx,
	 Position* min_pos, int min_value) {
  MacroBlock mb2 = fill_macro_block(frame, posy, posx);
  int diff = difference(*mb, mb2);
  if(diff < min_value)
    {
      min_value = diff;
      min_pos->x = posx;
      min_pos->y = posy;
    }
  return min_value;
}


int movey(BMP frame, MacroBlock* mb, int posy,
	  Position* min_pos, int min_value,
	   int limit_left, int limit_right)
{
  for(int i = mb->x; i >= limit_left; i--)
    {
      min_value = evaluate(frame, mb, posy, i, min_pos, min_value);
      if(min_value == 0) return min_value;
    }

  for(int i = mb->x; i <= limit_right; i++)
    {
      min_value = evaluate(frame, mb, posy, i, min_pos, min_value);
      if(min_value == 0) return min_value;
    }
  return min_value;
}

int movex(BMP frame, MacroBlock* mb, int posx,
	   Position* min_pos, int min_value,
	   int limit_up, int limit_down)
{
  for(int i = mb->y; i >= limit_up; i--)
    {
      min_value = evaluate(frame, mb, i, posx, min_pos, min_value);
      if(min_value == 0) return min_value;
    }

  for(int i = mb->y; i <= limit_down; i++)
    {
      min_value = evaluate(frame, mb, i, posx, min_pos, min_value);
      if(min_value == 0) return min_value;
    }
  return min_value;
}
