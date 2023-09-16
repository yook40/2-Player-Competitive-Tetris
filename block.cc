#include "block.h"

Block::Block(std::vector<std::vector<int>> pos, int level):
  level{level}, cellsLeft{4}, pos{pos}, lowerLeft{3, 0} {}

int Block::numCells() const { return cellsLeft; }

void Block::clearCell() { --cellsLeft; }

int Block::getLevel() const { return level; }

IBlock::IBlock(int level): Block{{{3, 0}, {3, 1}, {3, 2}, {3, 3}}, level} {}

JBlock::JBlock(int level): Block{{{2, 0}, {3, 0}, {3, 1}, {3, 2}}, level} {}

LBlock::LBlock(int level): Block{{{2, 2}, {3, 0}, {3, 1}, {3, 2}}, level} {}

OBlock::OBlock(int level): Block{{{2, 0}, {2, 1}, {3, 0}, {3, 1}}, level} {}

SBlock::SBlock(int level): Block{{{2, 1}, {2, 2}, {3, 0}, {3, 1}}, level} {}

ZBlock::ZBlock(int level): Block{{{2, 0}, {2, 1}, {3, 1}, {3, 2}}, level} {}

TBlock::TBlock(int level): Block{{{2, 0}, {2, 1}, {2, 2}, {3, 1}}, level} {}

// Board *Block::getBoard() const { return board; }

std::string IBlock::firstRow() const { return " IIII"; }

std::string IBlock::secondRow() const { return "     "; }

std::string JBlock::firstRow() const { return " J   "; }

std::string JBlock::secondRow() const { return " JJJ "; }

std::string LBlock::firstRow() const { return "   L "; }

std::string LBlock::secondRow() const { return " LLL "; }

std::string OBlock::firstRow() const { return " OO  "; }

std::string OBlock::secondRow() const { return " OO  "; }

std::string TBlock::firstRow() const { return " TTT "; }

std::string TBlock::secondRow() const { return "  T  "; }

std::string SBlock::firstRow() const { return "  SS "; }

std::string SBlock::secondRow() const { return " SS  "; }

std::string ZBlock::firstRow() const { return " ZZ  "; }

std::string ZBlock::secondRow() const { return "  ZZ "; }

std::string IBlock::getType() const { return std::string{"I"}; }

std::string JBlock::getType() const { return std::string{"J"}; }

std::string LBlock::getType() const { return std::string{"L"}; }

std::string SBlock::getType() const { return std::string{"S"}; }

std::string TBlock::getType() const { return std::string{"T"}; }

std::string OBlock::getType() const { return std::string{"O"}; }

std::string ZBlock::getType() const { return std::string{"Z"}; }


std::unique_ptr<Block> blockGen(char res, int level) {
  if (res == 'I') {
    return std::make_unique<IBlock>(level);
  } else if (res == 'J') {
    return std::make_unique<JBlock>(level);
  } else if (res == 'L') {
    return std::make_unique<LBlock>(level);
  } else if (res == 'T') {
    return std::make_unique<TBlock>(level);
  } else if (res == 'S') {
    return std::make_unique<SBlock>(level);
  } else if (res == 'Z') {
    return std::make_unique<ZBlock>(level);
  } else { 
    return std::make_unique<OBlock>(level);
  }
}

