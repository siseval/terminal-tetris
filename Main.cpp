#include <iostream>
#include "TetrisEngine.cpp"

using namespace std;

int main (int argc, char *argv[])
{
  TetrisEngine* tEngine = new TetrisEngine();
  tEngine->run();
  return 0;
}
