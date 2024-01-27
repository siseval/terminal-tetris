#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

namespace RasterLib 
{
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
  std::string syms = "  ";

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

  std::string draw()
  {
    return "\033[" + std::to_string(color + isLight) + 'm' + syms + "\033[" + std::to_string(Reset) + 'm';
  }  
  Color getColor()
  {
    return color;
  }
  void setColor(Color col)
  {
    color = col;
  }
  void setSyms(std::string s)
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
}



#endif 
