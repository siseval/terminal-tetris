#include <iostream>
#include "TetrisEngine.cpp"

using namespace std;

int main (int argc, char *argv[])
{
  std::cout << "\033]0;" << "Tetris" << "\007";
  TetrisEngine* tEngine = new TetrisEngine();
  tEngine->run();
  return 0;
}
