#include <Tetro.h>

Tetro::Tetro(Shape s)
  {
    shape = s;
    rotation = 0;

    switch (s)
    {
      case O:
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = RasterLib::Pixel::Yellow;
        break;

      case I:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[1][3] = 'X';
        color = RasterLib::Pixel::Cyan;
        break;
 
      case T:
        matrix[1][1] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = RasterLib::Pixel::White;
        break;

      case J:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][0] = 'X';
        color = RasterLib::Pixel::Blue;
        break;

      case L:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][2] = 'X';
        color = RasterLib::Pixel::Green;
        break;

      case Z:
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        color = RasterLib::Pixel::Purple;
        break;

      case S:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = RasterLib::Pixel::Red;
        break;
    }
  }


