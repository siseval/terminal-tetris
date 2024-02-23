#ifndef RASTER_H
#define RASTER_H

#include <Pixel.h>
#include <iostream>

namespace RasterLib
{
  class Raster
  {
      int width;
      int height;
      Pixel* pixels; 
  public:
      Raster()
      {

      }
      Raster(int w, int h)
      {
          width = w;
          height = h;

          pixels = new Pixel[w * h];
      }
      ~Raster()
      {
          delete pixels;
      }

      Pixel getPixel(int x, int y)
      {
          return pixels[width * y + x];
      }
      Pixel getPixel(int i)
      {
          return pixels[i];
      }

      void addRString(int x, int y, std::string s, Pixel::Color col = Pixel::Color::Reset)
      {
        int cCount = 0;
        for (int i = 0; i < s.size() / 2; i++)
        {
          std::string chars = std::string() + s[cCount] + s[cCount + 1];
          pixels[width * y + x + i].setSyms(chars);
          pixels[width * y + x + i].setSymCol(col);
          cCount += 2;
        }
      }
      void addHorRString(int x, int y, std::string s)
      {
        int cCount = 0;
        for (int i = 0; i < s.size() / 2; i++)
        {
          std::string chars = std::string() + s[cCount] + s[cCount + 1];
          pixels[width * (y + i) + x].setSyms(chars);
          cCount += 2;
        }
      }
      void setPixelLight(int x, int y)
      {
        pixels[width * y + x].setLight();
      }
       void setPixelDark(int x, int y)
      {
        pixels[width * y + x].setDark();
      }
      
      void setPixelSyms(int x, int y, std::string s)
      {
        getPixel(x, y).setSyms(s);
      }
      void setPixel(int x, int y, Pixel::Color col)
      {
        pixels[width * y + x].setColor(col);
      }
      void setPixel(int i, Pixel::Color col)
      {
          pixels[i].setColor(col);
      }
      void addHorLine(Pixel::Color col, int x, int y, int len)
      {
          int cx = x;

          for (int i = 0; i < len; i++)
          {
              setPixel(cx, y, col);
              cx += 1;
          }
      }
      void addVerLine(Pixel::Color col, int x, int y, int len)
      {
          int cy = y;

          for (int i = 0; i < len; i++)
          {
              setPixel(x, cy, col);
              cy += 1;
          }
      }
      void addBorder(Pixel::Color col)
      {
          addHorLine(col, 0, 0, width - 1);
          addHorLine(col, 0, height - 1, width);

          addVerLine(col, 0, 0, height - 1);
          addVerLine(col, width - 1, 0, height);
      }
      void clear()
      {
          for (int i = 0; i < width * height; i++)
          {
              pixels[i].setColor(Pixel::Reset);
              pixels[i].setDark();
          }
      }
      void draw()
      {
        std::string screen = "";
        for (int i = 0; i < width * height; i++)
        {
          screen += pixels[i].draw(); 
          if ((i + 1) % width == 0)
          {
            screen += '\n';
            screen += '\r';
          }
        }
        screen += '\n';
        screen += '\r';
        screen += "\x1B[2J\x1B[H";
        std::cout << screen;
      }
  };
}



#endif
