#include <TetrisEngine.h>

TetrisEngine::TetrisEngine()
  {
    raster = new RasterLib::Raster(12 + infoWidth, 22);

    for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w + 1; j++)
      {
        board[i][j] = 0;
      }
    }
  }
