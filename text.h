#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <string>

class GamePlay;

class TextDisplay {
  GamePlay *game;
  void printHeader(std::string, int) const;
  public:
  TextDisplay(GamePlay *);
  void display();
};

#endif
