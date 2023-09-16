#include "graphics.h"
#include "gameplay.h"
#include <string>
#include <sstream>

static Xwindow::Color typeToColor(std::string type) {
  if (type == "I") return Xwindow::Cyan;
  if (type == "J") return Xwindow::Blue;
  if (type == "L") return Xwindow::Orange;
  if (type == "O") return Xwindow::Yellow;
  if (type == "S") return Xwindow::Green;
  if (type == "T") return Xwindow::Magenta;
  if (type == "Z") return Xwindow::Red;
  if (type == " ") return Xwindow::White;
  if (type == "*") return Xwindow::Brown;
  return Xwindow::Black;
}

Graphics::Graphics(GamePlay *game): window{std::make_unique<Xwindow>(500, 700)},
  game{game}, board{game->boards[0].board, game->boards[1].board} {
  window->fillRectangle(10, 210, 2, 364, Xwindow::Black);
  window->fillRectangle(10, 210, 224, 2, Xwindow::Black);
  window->fillRectangle(232, 210, 2, 364, Xwindow::Black);
  window->fillRectangle(10, 572, 224, 2, Xwindow::Black);
  // second window
  window->fillRectangle(250, 210, 2, 364, Xwindow::Black);
  window->fillRectangle(250, 210, 224, 2, Xwindow::Black);
  window->fillRectangle(472, 210, 2, 364, Xwindow::Black);
  window->fillRectangle(250, 572, 224, 2, Xwindow::Black);
  window->drawString(15, 593, "Next:");
  window->drawString(255, 593, "Next:");
  for (int i = 2; i <= 3; ++i) {
    for (int j = 0; j < 4; ++j) {
      window->fillRectangle(12 + j * 20, 212 + i * 20, 20, 20, typeToColor(board[0].at(i).at(j).getType()));
      window->fillRectangle(252 + j * 20, 212 + i * 20, 20, 20, typeToColor(board[1].at(i).at(j).getType()));
    }
  }
  for (int i = 0; i < 4; ++i) {
    int x = game->getNextPos().at(i).at(1);
    int y = game->getNextPos().at(i).at(0);
    window->fillRectangle(15 + 20 * x, 600 + (y - 2) * 20, 20, 20, typeToColor(game->getNextType()));
  }
  window->drawString(20, 100, "Player 1 Level:");
  std::ostringstream oss;
  oss << game->playerLevels[0];
  window->drawString(20, 115, oss.str());
  window->drawString(270, 100, "Player 2 Level:");
  std::ostringstream oss1;
  oss1 << game->playerLevels[1];
  window->drawString(270, 115, oss1.str());
  window->drawString(20, 150, "Player 1 Score:");
  window->drawString(20, 170, "0");
  window->drawString(270, 150, "Player 2 Score:");
  window->drawString(270, 170, "0");
  window->drawString(200, 50, "hi score:");
  window->drawString(260, 51, "0");
};

void Graphics::updateBoard() {
  for (int i = 0; i < game->boards[0].numRows; ++i) {
    for (int j = 0; j < game->boards[0].numCols; ++j) {
      if (i >= 5 && i <= 14 && j >= 2 && j <= 8 && game->boards[game->curPlayer].blind) {
        window->fillRectangle((game->curPlayer ? 252 : 12) + j * 20, 212 + i * 20, 20, 20, Xwindow::Black);
      } else {
        Cell &cell1 = board[game->curPlayer].at(i).at(j);
        Cell &cell2 = game->boards[game->curPlayer].board.at(i).at(j);
        Cell &cell3 = board[!game->curPlayer].at(i).at(j);
        Cell &cell4 = game->boards[!game->curPlayer].board.at(i).at(j);
        if (cell1.getType() != cell2.getType()) {
          cell1.updateBlock(cell2.getBlock(), cell2.getType());
          window->fillRectangle((game->curPlayer ? 252 : 12) + j * 20, 212 + i * 20, 20, 20, typeToColor(cell1.getType()));
        }
        if (cell3.getType() != cell4.getType()) {
          cell3.updateBlock(cell4.getBlock(), cell4.getType());
          window->fillRectangle((game->curPlayer ? 12 : 252) + j * 20, 212 + i * 20, 20, 20, typeToColor(cell3.getType()));
        }
      }
    }
  }
  window->fillRectangle((game->curPlayer ? 15 : 255), 600, 80, 40, Xwindow::White);
  for (int i = 0; i < 4; ++i) {
    int x = game->getNextPos().at(i).at(1);
    int y = game->getNextPos().at(i).at(0);
    window->fillRectangle((game->curPlayer ? 255 : 15) + 20 * x, 600 + (y - 2) * 20, 20, 20, typeToColor(game->getNextType()));
  }
  {
  std::ostringstream oss;
  oss << game->playerScores[0];
  window->fillRectangle(20, 161, 50, 10, Xwindow::White);
  window->drawString(20, 170, oss.str());
  std::ostringstream oss1;
  oss1 << game->playerScores[1];
  window->fillRectangle(270, 161, 50, 10, Xwindow::White);
  window->drawString(270, 170, oss1.str());
  window->fillRectangle(259, 42, 50, 10, Xwindow::White);
  window->drawString(260, 51, (game->playerScores[0] > game->playerScores[1] ? oss.str() : oss1.str()));
  }
  std::ostringstream oss;
  oss << game->playerLevels[0];
  window->fillRectangle(19, 106, 50, 10, Xwindow::White);
  window->drawString(20, 115, oss.str());
  std::ostringstream oss1;
  oss1 << game->playerLevels[1];
  window->fillRectangle(270, 106, 50, 10, Xwindow::White);
  window->drawString(270, 115, oss1.str());
}

