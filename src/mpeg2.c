#include "mpeg2.h"
#include <limits.h>

MacroBlock fill_macro_block(BMP frame, int x, int y);
Position search_macro_block(BMP *frame, MacroBlock *mb);
int movey(BMP* frame, MacroBlock* mb, int posy,
	  Position* min_pos, int min_value,
	  int limit_left, int limit_right);

int movex(BMP* frame, MacroBlock* mb, int posx,
	   Position* min_pos, int min_value,
	  int limit_up, int limit_down);


MotionVector calc_motion_vector(BMP* frame1, BMP* frame2) {
  int num_blocks_y = frame1->height / 16;
  int num_blocks_x = frame1->width / 16;
  MotionVector mv = create_motion_blocks(num_blocks_y, num_blocks_x);

  for(int j = 0; j < num_blocks_y; j++) {
      for(int i = 0; i < num_blocks_x; i++) {
	MacroBlock mb = fill_macro_block(*frame1, i, j);
	Position new_pos = search_macro_block(frame2, &mb);
	mv.macro_blocks[j][i] = new_pos;
	}
    }
  return mv;
}


MacroBlock fill_macro_block(BMP frame, int x, int y) {
  MacroBlock mb;
  mb.x = x*16;
  mb.y = y*16;
  
  for(int i = 0; i < 16; i++) {
      for(int j = 0; j <16; j++) {
	  mb.block[i][j] = frame.pixels[x*16 + i][y*16 + j];
	}
    }
  return mb;
}


Position search_macro_block(BMP *frame, MacroBlock *mb) {
  int last_blocky = frame->height - 16;
  int last_blockx = frame->width - 16;
  Position min_pos;
  int min_value = INT_MAX;
  
  int up, down;
  up = down = mb->x;
  int left, right;
  left = right = mb->y;
  char can_up, can_down, can_left, can_right, can_move;
  can_up = can_down = can_left = can_right = can_move = 0;

  if(up != 0){
      can_up = 1;
      can_move++;
  }
  if(down < last_blocky-1) {
    can_down = 1;
    can_move++;
  }
  if(left != 0) {
    can_left = 1;
    can_move++;
  }
  if(right < last_blockx-1) {
    can_right = 1;
    can_move++;
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


int movey(BMP* frame, MacroBlock* mb, int posy,
	  Position* min_pos, int min_value,
	   int limit_left, int limit_right)
{
  for(int i = mb->x; i >= limit_left; i--)
    {
      MacroBlock mb2 = fill_macro_block(*frame, i, posy);
      int diff = difference(*mb, mb2);
      if(diff == 0)
	{
	  min_value = 0;
	  min_pos->x = i;
	  min_pos->y = posy;
	  return min_value;
	}
      else if(diff < min_value)
	{
	  min_value = diff;
	  min_pos->x = i;
	  min_pos->y = posy;
	}
    }

  for(int i = mb->x; i <= limit_right; i++)
    {
      MacroBlock mb2 = fill_macro_block(*frame, i, posy);
      int diff = difference(*mb, mb2);
      if(diff == 0)
	{
	  min_value = 0;
	  min_pos->x = i;
	  min_pos->y = posy;
	}
      else if(diff < min_value)
	{
	  min_value = diff;
	  min_pos->x = i;
	  min_pos->y = posy;
	}
    }
  return min_value;
}

int movex(BMP* frame, MacroBlock* mb, int posx,
	   Position* min_pos, int min_value,
	   int limit_up, int limit_down)
{
  for(int i = mb->y; i >= limit_up; i--)
    {
      MacroBlock mb2 = fill_macro_block(*frame, posx, i);
      int diff = difference(*mb, mb2);
      if(diff == 0)
	{
	  min_value = 0;
	  min_pos->x = posx;
	  min_pos->y = i;
	  return min_value;
	}
      else if(diff < min_value)
	{
	  min_value = diff;
	  min_pos->x = posx;
	  min_pos->y = i;
	}
    }

  for(int i = mb->y; i <= limit_down; i++)
    {
      MacroBlock mb2 = fill_macro_block(*frame, posx, i);
      int diff = difference(*mb, mb2);
      if(diff == 0)
	{
	  min_value = 0;
	  min_pos->x = posx;
	  min_pos->y = i;
	  return min_value;
	}
      else if(diff < min_value)
	{
	  min_value = diff;
	  min_pos->x = posx;
	  min_pos->y = i;
	}
    }
  return min_value;
}
