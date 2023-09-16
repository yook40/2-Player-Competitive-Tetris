#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_
#include <vector>
#include <memory>
#include "block.h"
#include "board.h"
#include "level.h"

class GamePlay {
  std::vector<Board> boards;
  std::vector<std::unique_ptr<Block>> blocksOnBoard[2];
  std::vector<std::unique_ptr<Level>> levels[2];
  int count[2];
  int playerLevels[2];
  int playerScores[2];
  bool curPlayer;
  bool dropped;
  void render(bool, size_t);
  std::vector<std::vector<int>> getNextPos() const;
  std::string getNextType() const;
  friend class TextDisplay;
  friend class Graphics;
  public:
    std::vector<std::string> names;
    std::vector<std::ifstream> files;
    std::string level3name[2];
    std::string level4name[2];
    std::ifstream level3file[2];
    std::ifstream level4file[2];
    GamePlay(std::string, std::string, int lvl1, int lvl2, unsigned int seed = 0);
    void notify();
    void levelUp();
    void levelDown();
    void rotateBlock(bool);
    bool shift(int, int, bool drop = false);
    void endTurn(bool);
    bool getPlayer() const;
    bool getState() const;
    int clearRows();
    int getCount() const;
    void putBlock(int, std::string);
    void updateScore(int);
    void replaceBlock(char);
    void setRandomness(bool);
    void noRandom(std::string);
    bool checkDropped() const;
};
#endif
