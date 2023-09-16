#include "gameplay.h"
#include "text.h"
#include "board.h"
#include "block.h"
#include <iostream>
#include <sstream>

TextDisplay::TextDisplay(GamePlay *gp): game{gp} {}

static int numDigits(int num) {
  if (!num) return 1;
  int res = 0;
  while (num) {
    ++res;
    num /= 10;
  }
  return res;
}

void TextDisplay::printHeader(std::string title, int num) const {
  std::cout << title;
  int numSpaces = 5 - numDigits(num);
  while (numSpaces) {
    std::cout << " ";
    --numSpaces;
  }
  std::cout << num;
}

void TextDisplay::display() {
  std::string separateHeader{"        "};
  std::string separateFooter{"                "};
  std::string emptyNext{"                     "};
  std::string border{"+-----------+"};
  auto bp1 = game->boards[0];
  auto bp2 = game->boards[1];
  printHeader(" hi score:", game->playerScores[0] > game->playerScores[1] ? game->playerScores[0] : game->playerScores[1]);
  std::cout << std::endl;
  printHeader(" Level:", game->playerLevels[0]);
  std::cout << separateHeader;
  printHeader(" Level:", game->playerLevels[1]);
  std::cout << std::endl;
  printHeader(" Score:", game->playerScores[0]);
  std::cout << separateHeader;
  printHeader(" Score:", game->playerScores[1]);
  std::cout << std::endl;
  std::cout << border << separateHeader << border << std::endl;
  for (int i = 0; i < bp1.numRows; ++i) {
    std::cout << '|';
    for (int c = 0; c < bp1.numCols; ++c) {
      if (c >= 2 && c <= 8 && i >= 5 && i <= 14 && game->boards[0].blind) {
        std::cout << '?';
      } else {
        std::cout << bp1.board.at(i).at(c).getType();
      }
    }
    std::cout << '|' << separateHeader << '|';
    for (int c = 0; c < bp2.numCols; ++c) {
      if (c >= 2 && c <= 8 && i >= 5 && i <= 14 && game->boards[1].blind) {
        std::cout << '?';
      } else {
        std::cout << bp2.board.at(i).at(c).getType();
      }
    }
    std::cout << '|' << std::endl;
  }
  std::cout << border << separateHeader << border << std::endl;
  std::cout << " Next:" << separateFooter << "Next:" << std::endl;
  if (!game->curPlayer) {
    std::cout << game->blocksOnBoard[game->curPlayer].back()->firstRow() << std::endl;
    std::cout << game->blocksOnBoard[game->curPlayer].back()->secondRow() << std::endl;
  } else {
    std::cout << emptyNext << game->blocksOnBoard[game->curPlayer].back()->firstRow() << std::endl;
    std::cout << emptyNext << game->blocksOnBoard[game->curPlayer].back()->secondRow() << std::endl;
  }
  std::cout << std::endl;
}

