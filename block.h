#ifndef _BLOCK_H_
#define _BLOCK_H_
#include <vector>
#include <string>
#include <memory>

class Block {
  int level;
  int cellsLeft;
  std::vector<std::vector<int>> pos;
  std::vector<int> lowerLeft;
  friend class GamePlay;
  public:
    virtual std::string getType() const = 0;
    Block(std::vector<std::vector<int>>, int);
    virtual std::string firstRow() const = 0;
    virtual std::string secondRow() const = 0;
    int numCells() const;
    void clearCell();
    int getLevel() const;
};

class IBlock: public Block {
  public:
    IBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class JBlock: public Block {
  public:
    JBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class LBlock: public Block {
  public:
    LBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class OBlock: public Block {
  public:
    OBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class SBlock: public Block {
  public:
    SBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class ZBlock: public Block {
  public:
    ZBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

class TBlock: public Block {
  public:
    TBlock(int);
    std::string getType() const override;
    std::string firstRow() const override;
    std::string secondRow() const override;
};

std::unique_ptr<Block> blockGen(char, int);
#endif
