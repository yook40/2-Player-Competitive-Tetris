#ifndef _CELL_H_
#define _CELL_H_
#include <string>
class Block;

class Cell {
  Block *block;
  std::string type;
  public:
  Cell();
  void updateBlock(Block *, std::string);
  std::string getType() const;
  Block *getBlock() const;
};
#endif
