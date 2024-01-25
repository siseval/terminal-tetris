#include "RasterLib/raster.cpp"

class Tetro
{
public:
  enum Shape{ O, I, T, J, L, Z, S };

private:
  Shape shape;
  char sChar;
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
  char getSChar()
  {
    return sChar;
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
        sChar = 'O';
        break;

      case I:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[1][3] = 'X';
        color = Pixel::Cyan;
        sChar = 'I';
        break;
 
      case T:
        matrix[1][1] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = Pixel::White;
        sChar = 'T';
        break;

      case J:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][0] = 'X';
        color = Pixel::Blue;
        sChar = 'J';
        break;

      case L:
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        matrix[1][2] = 'X';
        color = Pixel::Green;
        sChar = 'L';
        break;

      case Z:
        matrix[1][1] = 'X';
        matrix[1][2] = 'X';
        matrix[2][0] = 'X';
        matrix[2][1] = 'X';
        color = Pixel::Purple;
        sChar = 'Z';
        break;

      case S:
        matrix[1][0] = 'X';
        matrix[1][1] = 'X';
        matrix[2][1] = 'X';
        matrix[2][2] = 'X';
        color = Pixel::Red;
        sChar = 'S';
        break;
    }
  }
};
