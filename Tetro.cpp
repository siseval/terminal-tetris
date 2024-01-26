#include "RasterLib/raster.cpp"

class Tetro
{
public:
  enum Shape{ O, I, T, J, L, Z, S };

private:
  Shape shape;
  Pixel::Color color;
  int rotation = 0;

public:
  char matrix[4][4] =   
         {{ ' ', ' ', ' ', ' '},
          { ' ', ' ', ' ', ' '},
          { ' ', ' ', ' ', ' '},
          { ' ', ' ', ' ', ' '}};

  Pixel::Color getColor()
  {
    return color;
  }

  int getShape()
  {
    return shape;
  }

  void rotate()
  {
    if (rotation < 3)
    {
      rotation += 1;
    }
    else 
    {  
      rotation = 0; 
    }
  }
  int getRotation()
  {
    return rotation;
  }

  Tetro(Shape s)
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
        color = Pixel::Yellow;
        break;

      case I:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[1][3] = 'X';
        color = Pixel::Cyan;
        break;
 
      case T:
        matrix[1][1] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = Pixel::White;
        break;

      case J:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][0] = 'X';
        color = Pixel::Blue;
        break;

      case L:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][2] = 'X';
        color = Pixel::Green;
        break;

      case Z:
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        color = Pixel::Purple;
        break;

      case S:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = Pixel::Red;
        break;
    }
  }
};
