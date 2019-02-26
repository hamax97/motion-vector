#ifndef MACRO_BLOCK_H
#define MACRO_BLOCK_H

typedef struct {
  int x, y;
  int block [16][16];
} MacroBlock;

int difference(MacroBlock mb1, MacroBlock mb2);

#endif
