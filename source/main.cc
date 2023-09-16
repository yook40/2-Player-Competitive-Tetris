#include "text.h"
#include <random>
#include "gameplay.h"
#include <iostream>
#include <sstream>
#include "graphics.h"

std::string commands[12]{"drop", "down", "left", "right", "sequence", "random", "norandom", "clockwise", "counterclockwise", "levelup", "leveldown", "restart"};
std::string actions[3]{"blind", "heavy", "force"};
std::vector<char> types{'I', 'L', 'J', 'O', 'S', 'Z', 'T'};

std::string file1{"sequence1.txt"};
std::string file2{"sequence2.txt"};
int level = 0;
unsigned int seed = 0;
std::unique_ptr<Graphics> window{nullptr};
std::vector<GamePlay> game;

void execFile(std::string name, TextDisplay &td) {
  int gameover = 0;
  bool player = 0;
  std::ifstream file{name};
  std::string cmd;
  while (file >> cmd) {
    int multiplier;
    std::istringstream iss{cmd};
    if (!(iss >> multiplier)) {
      multiplier = 1;
    }
    for (size_t i = 0; i < cmd.length(); ++i) {
      if (!(cmd[i] >= '0' && cmd[i] <= '9')) {
        cmd = cmd.substr(i, cmd.length() - i);
        break;
      }
    }
    int count = 0;
    int index = 0;
    for (int i = 0; i < 11; ++i) {
      if (commands[i].substr(0, cmd.length()) == cmd) {
        ++count;
        index = i;
      }
    }
    if (count <= 1) {
      cmd = count ? commands[index] : cmd;
      while (multiplier) {
        if (cmd.length() == 1 && std::find(types.begin(), types.end(), cmd[0]) != types.end()) {
          multiplier = 1;
          game[0].replaceBlock(cmd[0]);
        } else if (cmd == "drop") {
          while(game[0].shift(0, 1, true));
          if (multiplier > 1) {
            try {
              game[0].endTurn(false);
              td.display();
            } catch (bool p) {
              gameover = 1;
              player = p;
            }
          }
        } else if (cmd == "clockwise") {
          game[0].rotateBlock(1);
        } else if (cmd == "counterclockwise") {
          game[0].rotateBlock(0);
        } else if (cmd == "right") {
          game[0].shift(1, 0);
        } else if (cmd == "left") {
          game[0].shift(-1, 0);
        } else if (cmd == "down") {
          game[0].shift(0, 1, false);
        } else if (cmd == "levelup") {
          game[0].levelUp();
        } else if (cmd == "leveldown") {
          game[0].levelDown();
        } else if (cmd == "norandom") {
          multiplier = 1;
          std::cin >> cmd;
          game[0].noRandom(cmd);
        } else if (cmd == "random") {
          multiplier = 1;
          game[0].setRandomness(true);
        } else if (cmd == "restart") {
          multiplier = 1;
          game.pop_back();
          game.emplace_back(file1, file2, level, level, seed);
        } else if (cmd == "sequence") {
          multiplier = 1;
          std::cin >> cmd;
          execFile(cmd, td);
        }
        if (cmd == "drop" || cmd == "right" || cmd == "left" || cmd == "down" ||
            cmd == "clockwise" || cmd == "counterclockwise") {
          try {
            game[0].notify();
          } catch (bool p) {
            gameover = 1;
            player = p;
          }
        }
        if (gameover) {
          td.display();
          if (window) {
            window->updateBoard();
          }
          throw player;
        }
        std::cout << std::endl;
        --multiplier;
      }
      if (game[0].checkDropped()) {
        try {
          game[0].endTurn(true);
        } catch (bool player) {
          td.display();
          if (window) {
            window->updateBoard();
          }
          throw player;
        }
      }
      td.display();
      if (window) {
        window->updateBoard();
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int gameover = 0;
  bool player = 0;
  bool graphics = true;
  for (int i = 1; i < argc; i += 2) {
    std::string arg{argv[i]};
    if (arg == "-text") {
      --i;
      graphics = false;
    } else {
      std::istringstream iss{argv[i + 1]};
      if (arg == "-startlevel") {
        iss >> level;
      } else if (arg == "-scriptfile1") {
        iss >> file1;
      } else if (arg == "-scriptfile2") {
        iss >> file2;
      } else if (arg == "-seed") {
        iss >> seed;
      }
    }
  }
  srand(seed);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 11);
  game.emplace_back(file1, file2, level, level, seed);
  TextDisplay td{&game[0]};
  std::string cmd;
  td.display();
  if (graphics) {
    window = std::make_unique<Graphics>(&game[0]);
  }
  while (std::cin >> cmd) {
    int multiplier;
    std::istringstream iss{cmd};
    if (!(iss >> multiplier)) {
      multiplier = 1;
    }
    for (size_t i = 0; i < cmd.length(); ++i) {
      if (!(cmd[i] >= '0' && cmd[i] <= '9')) {
        cmd = cmd.substr(i, cmd.length() - i);
        break;
      }
    }
    int count = 0;
    int index = 0;
    for (int i = 0; i < 11; ++i) {
      if (commands[i].substr(0, cmd.length()) == cmd) {
        ++count;
        index = i;
      }
    }
    if (count <= 1) {
      cmd = count ? commands[index] : cmd;
      while (multiplier) {
        if (cmd.length() == 1 && std::find(types.begin(), types.end(), cmd[0]) != types.end()) {
          multiplier = 1;
          game[0].replaceBlock(cmd[0]);
        } else if (cmd == "drop") {
          while(game[0].shift(0, 1, true));
          if (multiplier > 1) {
            try {
              game[0].endTurn(false);
              td.display();
            } catch (bool p) {
              gameover = 1;
              player = p;
            }
          }
        } else if (cmd == "clockwise") {
          game[0].rotateBlock(1);
        } else if (cmd == "counterclockwise") {
          game[0].rotateBlock(0);
        } else if (cmd == "right") {
          game[0].shift(1, 0);
        } else if (cmd == "left") {
          game[0].shift(-1, 0);
        } else if (cmd == "down") {
          game[0].shift(0, 1, false);
        } else if (cmd == "levelup") {
          game[0].levelUp();
        } else if (cmd == "leveldown") {
          game[0].levelDown();
        } else if (cmd == "norandom") {
          multiplier = 1;
          std::cin >> cmd;
          game[0].noRandom(cmd);
        } else if (cmd == "random") {
          multiplier = 1;
          game[0].setRandomness(true);
        } else if (cmd == "restart") {
          multiplier = 1;
          game.pop_back();
          game.emplace_back(file1, file2, level, level, seed);
        } else if (cmd == "sequence") {
          std::cin >> cmd;
          try {
            execFile(cmd, td);
          } catch (bool p) {
            gameover = 1;
            player = p;
          }
        }
        if (cmd == "drop" || cmd == "right" || cmd == "left" || cmd == "down" ||
            cmd == "clockwise" || cmd == "counterclockwise") {
          try {
            game[0].notify();
          } catch (bool p) {
            gameover = 1;
            player = p;
          }
        }
        if (gameover) {
          td.display();
          if (window) {
            window->updateBoard();
          }
          std::cout << "Player " << player + 1 << " won!" << std::endl;
          return 0;
        }
        std::cout << std::endl;
        --multiplier;
      }
      if (game[0].checkDropped()) {
        try {
          game[0].endTurn(true);
        } catch (bool player) {
          td.display();
          if (window) {
            window->updateBoard();
          }
          std::cout << "Player " << player + 1 << " won!" << std::endl;
          return 0;
        }
      }
      td.display();
      if (window) {
        window->updateBoard();
      }
    }
  }
}

