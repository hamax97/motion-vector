#include "macro_block.h"
#include <stdlib.h>

int difference(MacroBlock mb1, MacroBlock mb2) {
  int diff = 0;
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++) {
      diff += abs(mb1.block[i][j] - mb2.block[i][j]);
    }
  }
  return diff;
}
