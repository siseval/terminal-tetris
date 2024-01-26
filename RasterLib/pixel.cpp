#include <iostream>
using namespace std;

class Pixel
{
public:
  enum Color
	{
		Reset = 0,
		Black = 40,
		Red = 41,
		Green = 42,
		Yellow = 43,
		Blue = 44,
		Purple = 45,
		Cyan = 46,
		White = 47,
	};

private:
  Color color;
  string syms = "  ";

  int isLight = 0;

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
    return "\033[" + to_string(color + isLight) + 'm' + syms + "\033[" + to_string(Reset) + 'm';
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
  void setLight()
  {
    isLight = 60;
  }
  void setDark()
  {
    isLight = 0;
  }
};
