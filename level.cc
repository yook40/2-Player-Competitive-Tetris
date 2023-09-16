#include "gameplay.h"
#include <iostream>

Level::Level(GamePlay *game): game{game} {}

Level0::Level0(GamePlay *game): Level{game} {}

Level1::Level1(GamePlay *game): Level{game} {}

Level2::Level2(GamePlay *game): Level{game} {}

Level4::Level4(GamePlay *game): Level{game}, random{true} {}

Level3::Level3(GamePlay *game): Level{game}, random{true} {}

void Level::setRandomness(bool) {}

void Level::setFile(std::string) {}

void Level::effect() const {}

std::unique_ptr<Block> Level0::newBlock() {
  char res;
  if (!(game->files[game->getPlayer()] >> res)) {
    game->files[game->getPlayer()] = std::ifstream{game->names[game->getPlayer()]};
    game->files[game->getPlayer()] >> res;
  }
  return blockGen(res, 0);
}

std::unique_ptr<Block> Level1::newBlock() {
  char type[12] = {'I', 'I', 'J', 'J', 'L', 'L', 'T', 'T', 'S', 'Z', 'O', 'O'};
  char res = type[rand() % 12];
  return blockGen(res, 1);
}

std::unique_ptr<Block> Level2::newBlock() {
  char type[7] = {'I', 'J', 'L', 'T', 'S', 'Z', 'O'};
  char res = type[rand() % 7];
  return blockGen(res, 2);
}

std::unique_ptr<Block> Level3::newBlock() {
  char res;
  if (random) {
    char type[9] = {'I', 'J', 'L', 'T', 'S', 'S', 'Z', 'Z', 'O'};
    res = type[rand() % 9];
  } else {
    if (!(game->level3file[game->getPlayer()] >> res)) {
      game->level3file[game->getPlayer()] = std::ifstream{game->level3name[game->getPlayer()]};
      game->level3file[game->getPlayer()] >> res;
    }
  }
  return blockGen(res, 3);
}

std::unique_ptr<Block> Level4::newBlock() {
  char res;
  if (random) {
    char type[9] = {'I', 'J', 'L', 'T', 'S', 'S', 'Z', 'Z', 'O'};
    res = type[rand() % 9];
  } else {
    if (!(game->level4file[game->getPlayer()] >> res)) {
      game->level4file[game->getPlayer()] = std::ifstream{game->level4name[game->getPlayer()]};
      game->level4file[game->getPlayer()] >> res;
    }
  }
  return blockGen(res, 4);
}

void Level3::setRandomness(bool r) { random = r; }

void Level4::setRandomness(bool r) { random = r; }

void Level3::setFile(std::string f) {
  game->level3name[game->getPlayer()] = f;
  game->level3file[game->getPlayer()] = std::ifstream{f};
}

void Level4::setFile(std::string f) {
  game->level4name[game->getPlayer()] = f;
  game->level4file[game->getPlayer()] = std::ifstream{f};
}

void Level3::effect() const {
  if (!game->getState()) {
    game->shift(0, 1, false);
  }
}

void Level4::effect() const {
  if (game->getState()) {
    if (game->getCount() && !(game->getCount() % 5)) {
      game->putBlock(5, "*");
    }
  } else {
    game->shift(0, 1, false);
  }
}

