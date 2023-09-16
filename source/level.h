#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "block.h"
#include <fstream>

class GamePlay;

class Level {
  protected:
  int level;
  GamePlay *game;
  public:
  Level(GamePlay *);
  virtual std::unique_ptr<Block> newBlock() = 0;
  virtual void effect() const;
  virtual void setRandomness(bool);
  virtual void setFile(std::string);
};

class Level0: public Level {
  public:
  Level0(GamePlay *);
  std::unique_ptr<Block> newBlock() override;
};

class Level1: public Level {
  public:
  Level1(GamePlay *);
  std::unique_ptr<Block> newBlock() override;
};

class Level2: public Level {
  public:
  Level2(GamePlay *);
  std::unique_ptr<Block> newBlock() override;
};

class Level3: public Level {
  bool random;
  public:
  Level3(GamePlay *);
  void setRandomness(bool) override;
  void setFile(std::string) override;
  std::unique_ptr<Block> newBlock() override;
  void effect() const override;
};

class Level4: public Level {
  bool random;
  public:
  Level4(GamePlay *);
  void setRandomness(bool) override;
  void setFile(std::string) override;
  std::unique_ptr<Block> newBlock() override;
  void effect() const override;
};
#endif
