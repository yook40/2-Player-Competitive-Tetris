#ifndef _BOARD_H_
#define _BOARD_H_
#include <vector>
#include "cell.h"

struct Board {
  int numRows;
  int numCols;
  bool random;
  bool blind;
  bool heavy;
  char forceType;
  Board();
  std::vector<std::vector<Cell>> board;
};

#endif
