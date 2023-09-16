#include "board.h"


Board::Board(): numRows{18}, numCols{11}, random{true}, blind{false},
  heavy{false}, board(numRows, std::vector<Cell>(numCols, Cell{})) {}

