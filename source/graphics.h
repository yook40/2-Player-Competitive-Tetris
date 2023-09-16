#include "window.h"
#include <memory>
#include "cell.h"
#include <vector>

class GamePlay;

class Graphics {
  std::unique_ptr<Xwindow> window;
  GamePlay *game;
  std::vector<std::vector<Cell>> board[2];
  public:
  Graphics(GamePlay *);
  void updateBoard();
};


