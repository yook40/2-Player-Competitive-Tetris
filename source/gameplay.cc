#include "gameplay.h"
#include <iostream>

const std::string RED{"\033[31m"};

const std::string RESET{"\033[0m"};

GamePlay::GamePlay(std::string file1, std::string file2, int lvl1, int lvl2, unsigned int seed):
  boards{Board{}, Board{}}, count{1, 1}, playerLevels{lvl1, lvl2}, playerScores{0, 0},
  curPlayer{0}, dropped{false}, names{file1, file2} {
  files.emplace_back(std::ifstream{file1});
  files.emplace_back(std::ifstream{file2});
  for (int i = 0; i < 2; ++i) {
    levels[i].emplace_back(std::make_unique<Level0>(this));
    levels[i].emplace_back(std::make_unique<Level1>(this));
    levels[i].emplace_back(std::make_unique<Level2>(this));
    levels[i].emplace_back(std::make_unique<Level3>(this));
    levels[i].emplace_back(std::make_unique<Level4>(this));
  }
  blocksOnBoard[0].emplace_back(levels[0].at(playerLevels[0])->newBlock());
  // player 1's active block
  blocksOnBoard[1].emplace_back(levels[1].at(playerLevels[1])->newBlock());
  // get player 2's block ready on board
  blocksOnBoard[0].emplace_back(levels[0].at(playerLevels[0])->newBlock());
  // player 1's next block
  render(0, 0);
  render(1, 0);
}

bool GamePlay::checkDropped() const { return dropped; }

static bool isFull(const std::vector<Cell> &row) {
  for (auto c : row) {
    if (c.getType() == " ") {
      return false;
    }
  }
  return true;
}

std::vector<std::vector<int>> GamePlay::getNextPos() const {
  return blocksOnBoard[curPlayer].back()->pos;
}

std::string GamePlay::getNextType() const {
  return blocksOnBoard[curPlayer].back()->getType();
}

void GamePlay::setRandomness(bool rand) {
  levels[curPlayer][playerLevels[curPlayer]]->setRandomness(rand);
}

void GamePlay::noRandom(std::string file) {
  setRandomness(false);
  levels[curPlayer][playerLevels[curPlayer]]->setFile(file);
}

void GamePlay::render(bool player, size_t index) {
  Block *block = blocksOnBoard[player].at(index).get();
  bool gameover = false;
  for (int i = 0; i < 4; ++i) {
    Cell &cell = boards[player].board.at(block->pos[i][0]).at(block->pos[i][1]);
    if (cell.getType() != " ") {
      gameover = true;
      cell.updateBlock(block, RED + block->getType() + RESET);
    } else {
      cell.updateBlock(block, block->getType());
    }
  }
  if (gameover) {
    throw !player;
  }
}

void GamePlay::updateScore(int score) { playerScores[curPlayer] += score; }

void GamePlay::notify() { levels[curPlayer][playerLevels[curPlayer]]->effect(); }

bool GamePlay::getState() const { return dropped; }

bool GamePlay::getPlayer() const { return curPlayer; }

void GamePlay::levelUp() {
  int i = levels[0].size() - 1;
  if (playerLevels[curPlayer] < i) {
    ++playerLevels[curPlayer];
  }
}

void GamePlay::levelDown() {
  if (playerLevels[curPlayer] > 0) {
    --playerLevels[curPlayer];
  }
}

void GamePlay::replaceBlock(char type) {
  size_t index = blocksOnBoard[curPlayer].size() - 2;
  Block *block = blocksOnBoard[curPlayer].at(index).get();
  std::vector<std::vector<int>> &pos = block->pos;
  bool replace = true;
  for (int i = 0; i < 4; ++i) {
    boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(nullptr, " ");
  }
  std::unique_ptr<Block> newBlock{blockGen(type, playerLevels[curPlayer])};
  for (int i = 0; i < 4; ++i) {
    int x = newBlock->pos.at(i).at(1) - newBlock->lowerLeft.at(1) + block->lowerLeft.at(1);
    int y = newBlock->pos.at(i).at(0) - newBlock->lowerLeft.at(0) + block->lowerLeft.at(0);
    if (x > 10 || boards[curPlayer].board.at(y).at(x).getType() != " ") {
      replace = false;
    }
    newBlock->pos.at(i) = std::vector<int>{y, x};
  }
  if (replace) {
    newBlock->lowerLeft = block->lowerLeft;
    std::swap(newBlock, blocksOnBoard[curPlayer].at(index));
  }
  render(curPlayer, index);
}

void GamePlay::endTurn(bool sw) {
  dropped = false;
  boards[curPlayer].blind = false;
  boards[curPlayer].heavy = false;
  boards[curPlayer].random = true;
  int rowsCleared = clearRows();
  if (!rowsCleared) {
    ++count[curPlayer];
  } else {
    if (rowsCleared >= 2) {
      std::string action;
      std::cout << "Please choose a special action [blind/force/heavy]: ";
      std::cin >> action;
      if (action == "blind") {
        boards[!curPlayer].blind = true;
      } else if (action == "force") {
        std::cin >> boards[!curPlayer].forceType;
        boards[!curPlayer].random = false;
      } else if (action == "heavy") {
        boards[!curPlayer].heavy = true;
      }
    }
    count[curPlayer] = 1;
  }
  if (sw) {
    curPlayer = !curPlayer;
  }
  blocksOnBoard[curPlayer].emplace_back(levels[curPlayer].at(playerLevels[curPlayer])->newBlock());
  if (!boards[curPlayer].random) {
    std::unique_ptr<Block> newBlock{blockGen(boards[curPlayer].forceType, playerLevels[curPlayer])};
    Block *oldBlock = blocksOnBoard[curPlayer].at(blocksOnBoard[curPlayer].size() - 2).get();
    for (int i = 0; i < 4; ++i) {
      boards[curPlayer].board.at(oldBlock->pos.at(i).at(0)).at(oldBlock->pos.at(i).at(1)).updateBlock(nullptr, " ");
    }
    std::swap(newBlock, blocksOnBoard[curPlayer].at(blocksOnBoard[curPlayer].size() - 2));
  }
  if (sw) {
    if (!boards[curPlayer].random) {
      render(curPlayer, blocksOnBoard[curPlayer].size() - 2);
      // if the block for the new player is forced then render the forced block as well
    }
    render(!curPlayer, blocksOnBoard[!curPlayer].size() - 1);
    // if switch player then get previous player's next block ready on board
  } else {
    render(curPlayer, blocksOnBoard[curPlayer].size() - 2);
    // if not get the next block for the current player ready on board
  }
}

int GamePlay::clearRows() {
  std::vector<std::vector<Cell>> newBoard(18, std::vector<Cell>(11, Cell{}));
  int cur_row = 17;
  int rowsCleared = 0;
  for (int i = 17; i >= 0; --i) {
    if (!isFull(boards[curPlayer].board.at(i))) {
      newBoard.at(cur_row) = boards[curPlayer].board.at(i);
      --cur_row;
    } else {
      ++rowsCleared;
      for (auto c : boards[curPlayer].board.at(i)) {
        if (c.getBlock()) { // if it is a player placed block instead of * in the middle column
          c.getBlock()->clearCell();
          if (!c.getBlock()->numCells()) { // if all cells in this block are removed
            playerScores[curPlayer] += (c.getBlock()->getLevel() + 1) * (c.getBlock()->getLevel() + 1); // get score for this block
            auto block = blocksOnBoard[curPlayer].begin();
            while (block->get() != c.getBlock()) ++block;
            blocksOnBoard[curPlayer].erase(block); // find the block in blocksOnBoard[curPlayer] and erase it
          }
        }
      }
    }
  }
  boards[curPlayer].board = newBoard;
  if (rowsCleared) {
    playerScores[curPlayer] += (playerLevels[curPlayer] + rowsCleared) * (playerLevels[curPlayer] + rowsCleared);
  }
  return rowsCleared;
}

bool GamePlay::shift(int x, int y, bool drop) {
  size_t index = blocksOnBoard[curPlayer].size() - 2;
  Block *block = blocksOnBoard[curPlayer].at(index).get();
  // shift the second last block because the last one is the next block
  std::vector<std::vector<int>> &pos = block->pos;
  for (int i = 0; i < 4; ++i) {
    boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(nullptr, " ");
  }
  for (int i = 0; i < 4; ++i) {
    if (pos[i][0] + y >= boards[0].numRows || pos[i][1] + x < 0 || pos[i][1] + x >= boards[0].numCols ||
        boards[curPlayer].board.at(pos[i][0] + y).at(pos[i][1] + x).getType() != " ") {
      if (y && drop) {
        dropped = true;
      }
      for (int i = 0; i < 4; ++i) {
        boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(block, block->getType());
      }
      return false;
    }
  }
  block->lowerLeft[0] += y;
  block->lowerLeft[1] += x;
  for (int i = 0; i < 4; ++i) {
    block->pos[i][0] += y;
    block->pos[i][1] += x;
    boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(block, block->getType());
  }
  if (boards[curPlayer].heavy && x && !y) { // if the block moves left or right and heavy is true
    for (int i = 0; i < 4; ++i) {
      boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(nullptr, " ");
    }
    for (int i = 0; i < 2; ++i) {
      if (pos[i][0] + 1 >= boards[0].numRows || boards[curPlayer].board.at(pos[i][0] + 1).at(pos[i][1]).getType() != " ") {
        for (int j = 0; j < 4; ++j) { // if cannot be moved anymore then restore the cells
          boards[curPlayer].board.at(pos[j][0]).at(pos[j][1]).updateBlock(block, block->getType());
        }
        dropped = true; // heavy action is considered drop
        return false;
      }
      ++block->lowerLeft[0];
      for (int j = 0; j < 4; ++j) { // heavy moves block down so only increment [0]
        ++block->pos[j][0];
      }
    }
    for (int i = 0; i < 4; ++i) {
      boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(block, block->getType());
    }
  }
  return true;
}

void GamePlay::rotateBlock(bool clockwise) {
  size_t index = blocksOnBoard[curPlayer].size() - 2;
  Block *block = blocksOnBoard[curPlayer].at(index).get();
  std::vector<std::vector<int>> &pos = block->pos;
  int dest[4][2];
  if (clockwise) {
    for (int i = 0; i < 4; ++i) {
      dest[i][0] = pos[i][1];
      dest[i][1] = -pos[i][0];
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      dest[i][0] = -pos[i][1];
      dest[i][1] = pos[i][0];
    }
  }
  int newLowerLeft[2] = {dest[0][0], dest[0][1]};
  for (int i = 0; i < 4; ++i) {
    if (dest[i][0] > newLowerLeft[0]) {
      newLowerLeft[0] = dest[i][0];
    }
    if (dest[i][1] < newLowerLeft[1]) {
      newLowerLeft[1] = dest[i][1];
    }
  }
  for (int i = 0; i < 4; ++i) {
    boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(nullptr, " ");
  }
  for (int i = 0; i < 4; ++i) {
    dest[i][0] += block->lowerLeft[0] - newLowerLeft[0];
    dest[i][1] += block->lowerLeft[1] - newLowerLeft[1];
    if (dest[i][1] >= boards.at(0).numCols || boards[curPlayer].board.at(dest[i][0]).at(dest[i][1]).getType() != " ") {
      for (int i = 0; i < 4; ++i) {
        boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(block, block->getType());
      }
      return;
    }
  }
  for (int i = 0; i < 4; ++i) {
    block->pos[i][0] = dest[i][0];
    block->pos[i][1] = dest[i][1];
    boards[curPlayer].board.at(pos[i][0]).at(pos[i][1]).updateBlock(block, block->getType());
  }
}

int GamePlay::getCount() const {
  return count[curPlayer];
}

void GamePlay::putBlock(int col, std::string type) {
  for (int i = 3; i < 18; ++i) {
    if (boards[curPlayer].board.at(i).at(5).getType() != " ") {
      if (i > 3) {
        boards[curPlayer].board.at(i - 1).at(col).updateBlock(nullptr, type);
        return;
      } else {
        throw curPlayer;
      }
    }
  }
}

