#include <iostream>
using namespace std;

class Pixel
{
public:
  enum Color
	{
		Reset = 0,
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Purple = 35,
		Cyan = 36,
		White = 37
	};

private:
  Color color;
  string syms = "  ";

public:

  Pixel(Color col)
  {
    color = col;
  }

  Pixel()
  {
    color = Reset;
  }

  string draw()
  {
    return "\033[" + to_string(color + 10) + 'm' + syms + "\033[" + to_string(Reset) + 'm';
  }  
  Color getColor()
  {
    return color;
  }
  void setColor(Color col)
  {
    color = col;
  }
  void setSyms(string s)
  {
    syms = s;
  }
};
