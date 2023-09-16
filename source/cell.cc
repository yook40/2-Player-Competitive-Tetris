#include "cell.h"

Cell::Cell(): block{nullptr}, type{" "} {}

void Cell::updateBlock(Block *b, std::string t) {
  block = b;
  type = t;
}

std::string Cell::getType() const { return type; }

Block *Cell::getBlock() const { return block; }
