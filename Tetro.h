#ifndef TETRO_H
#define TETRO_H

#include "RasterLib/Raster.h"

class Tetro {
public:
  enum Shape { O, I, T, J, L, Z, S };

private:
  Shape shape;
  RasterLib::Pixel::Color color;
  int rotation = 0;

public:
  char matrix[4][4] = {{' ', ' ', ' ', ' '},
                       {' ', ' ', ' ', ' '},
                       {' ', ' ', ' ', ' '},
                       {' ', ' ', ' ', ' '}};

  RasterLib::Pixel::Color getColor() { return color; }

  int getShape() { return shape; }

  void rotate() {
    if (rotation < 3) {
      rotation += 1;
    } else {
      rotation = 0;
    }
  }
  int getRotation() { return rotation; }

  Tetro(Shape s);
};

#endif
