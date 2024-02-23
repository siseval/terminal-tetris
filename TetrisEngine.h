#ifndef TETRIS_ENGINE_H
#define TETRIS_ENGINE_H

#include <Tetro.h>
#include <unistd.h>
#include <curses.h>
#include <fstream>

class TetrisEngine
{
private:
  int queue[4] = {-1, -1, -1, -1};
  int held = -1;
  bool hasStored = false;
  Tetro curTetro = Tetro(Tetro::O);

  RasterLib::Raster* raster;

  const static int h = 20;
  const static int w = 10;

  int score = 0;
  int high = 0;
  int mult = 1;

  bool doAnim = true;
  bool showGhost = true;

  const static int infoWidth = 5;

  int board[h + 1][w + 1];

  int alteredRows[4] = {-1 , -1, -1, -1};

  int X = 4;
  int Y = 0;

  int pointsPerLine[4] = {2000, 3000, 5000, 8000};
  int pointsPerPlace = 80;

  bool lost = false;

  int lineFlashMs = 100;
  bool doingAnim = false;

  int lvl = 1;
  int maxLvl = 20;
  int msPassed = 0;
  int msToLvl = 40000;

  int ms = 60;
  int fallFrames = 22;
  int frameCount = 0;
  bool running = true;

public:
  TetrisEngine();
  ~TetrisEngine()
  {
    delete raster;
  }

  void run()
  {
    initCurses();
    readHigh();
    resetRaster();
    clearQueue();
    fillQueue();
    resetXY();
    getNewTetro();
    runLoop();

    printf("\e[?25l");
  }
private:
  void initCurses()
  {
    initscr();
    timeout(ms);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
  }
  void lose()
  {
    std::string topBar = saveHigh() ? "- NEW HIGH -" : "- YOU LOSE -";
    raster -> addRString(3, 0, topBar);
    raster -> addRString(3, 21, "r to restart");

    lost = true;
  }
  void readHigh()
  {
    std::ifstream scoreFile;
    scoreFile.open("highscore.txt");
    std::string sc; 
    scoreFile >> sc;

    if (sc == "")
    {
      sc = "0";
    }

    high = stoi(sc);
  }
  bool saveHigh()
  {
    if (high < score)
    {
      high = score;
      std::ofstream scoreFile;
      scoreFile.open("highscore.txt", std::ofstream::out | std::ofstream::trunc);
      scoreFile << std::to_string(score);
      return true;
    }
    return false;
  }
  void restart()
  {
    score = 0;
    mult = 1;
    raster -> addRString(3, 0, "            ");
    raster -> addRString(3, 21, "            ");
    lost = false;
    clearBoard();
    held = -1;
    lvl = 0;
    msPassed = 0;
    run();
  }
  void quit()
  {
    nocbreak();
    endwin();
    exit(0);
  }

  void runLoop()
  {
    while (running)
    {
      resetRaster();

      handleInput();
      
      drawBoard();
      drawCurTetro();
 
      updateInfo();
      
      raster -> draw();
      
      if (!lost)
      {
        countFrame(); 
      }
      handleTime(); 
    }
  }

  //  --- PLAYER CONTROLS ---  //

  void handleInput()
  {
    if (doingAnim)
    {
      return;
    }
    if (lost)
    {
      char c = getch();
      if (c == 'r')
      {
        restart();
      }
      if (c == 'q')
      {
        quit();
      }
    }
    else 
    {
      doAction(getch());
    }
  }
  void resetXY()
  {
    X = 4;
    Y = 0;
  }

  void doAction(int input)
  {
    switch (input)
    {
      case 'c':
        if (!hasStored)
        {
          storeCurTetro();
        }
        break;
      case ' ':
        slam();
        break;
      case KEY_UP:
      case 'z':
        rotateCurTetro();
        break;
      case KEY_RIGHT:
        move(1, 0);
        break;
      case KEY_LEFT:
        move(-1, 0);
        break;
      case KEY_DOWN:
        resetFrameCount();
        move(0, 1);
        break;
      case 'q':
        quit();
        break;
      case 'g':
        toggleGhost();
        break;
      case 'h':
        toggleAnim();
        break;
      default:
        break;
    }
  }

  void toggleGhost()
  {
    showGhost = !showGhost;
  }
  void toggleAnim()
  {
    doAnim = !doAnim;
  }
  void slam()
  {
    Y += getLowestY();
    placeCurTetro();
  }

  void move(int x, int y)
  {
    if (x != 0)
    {
      if (collidesRight(x) || collidesLeft(-x))
      {
        x = 0;
      }
    }
    if (y != 0)
    {
      if (collidesDown(y))
      {
        y = 0;

        if (Y < 0)
        {
          lose();
        }
        else 
        {
          placeCurTetro();
        }
      }
    }
    X += x;
    Y += y;
  }

  //  --- TETROS ---  //

  void spawnTetro(int sIndex)
  {
    curTetro = Tetro(static_cast<Tetro::Shape>(sIndex));
  } 

  void drawCurTetro()
  {
    int dy = getLowestY();
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (curTetro.matrix[i][j] == 'X')
        {
          raster -> setPixel(j + X, i + Y, curTetro.getColor());

          if (showGhost)
          {
            raster -> setPixel(j + X, i + Y + dy, curTetro.getColor());
            raster -> setPixelLight(j + X, i + Y + dy);
          }
        }
      }
    }
  }
  int getLowestY()
  {
    for (int i = 0; i < h; i++)
    {
      if (collidesDown(i))
      {
        return i - 1;
      }
    }
    return h - Y - 1;
  }

  void storeCurTetro()
  {
    hasStored = true;
    int temp = -1;
    if (held != -1)
    {
      temp = held;
    }
    held = curTetro.getShape();
    if (temp != -1)
    {
      spawnTetro(temp);
    }
    else 
    {  
      getNewTetro(); 
    }
    resetXY();
  }


  void placeCurTetro()
  {
    hasStored = false;
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (checkOOB(i + Y - 1))
        {
          break;
        }
        if (curTetro.matrix[i][j] == 'X')
        {
          if (board[i + Y - 1][j + X] != 0)
          {
            lose();
          }
          else 
          {
            board[i + Y - 1][j + X] = curTetro.getColor();
          }
        }
      }
    }
    checkLines();
    score += pointsPerPlace * mult; 
    getNewTetro();
  }

  void rotateCurTetro()
  {
    char matrix[4][4];
    
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        matrix[i][j] = curTetro.matrix[j][3-i];
      }
    }
 
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        curTetro.matrix[i][j] = matrix[i][j];
      }
    }
    if (Y < 4)
    {
      while (collidesUp())
      {
        Y += 1;
      }
    }
    while (collidesDown(0))
    {
      Y -= 1;
    }
    while (collidesRight(0))
    {
      if (collidesLeft(1))
      {
        Y -= 1;
      }
      else 
      {
        X -= 1;
      }
    }
    while (collidesLeft(0))
    {
      if (collidesRight(1))
      {
        Y -= 1;
      }
      else 
      {
        X += 1; 
      }
    }
        
    curTetro.rotate();
  }

  void checkCollision()
  {
    if (collidesLeft(0))
    {
      X += 1;
    }
    else if (collidesRight(0))
    {
      X -= 1;
    }
  }

  bool checkOOB(int y)
  {
    if (y < 0)
    {
      return true;
    }
    return false;
  }

  bool collidesRight(int dx)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (checkOOB(Y + (3 - i) - 1))
        {
          break;
        }
        if (curTetro.matrix[i][j] == 'X')
        {
          if (board[Y + i - 1][X + j + dx] != 0 || X + j + dx > 10)
          {
            return true;
          }
        }
      }
    }
    return false;
  }
  bool collidesLeft(int dx)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (checkOOB(Y + (3 - i) - 1))
        {
          break;
        }
        if (curTetro.matrix[i][j] == 'X')
        {
          if (board[Y + i - 1][X + j - dx] != 0 || X + j - dx < 1)
          {
            return true;
          }
        }
      }
    }
    return false;
  }
  bool collidesDown(int dy)
  {
    for (int i = 3; i >= 0; i--)
    {
      for (int j = 0; j < 4; j++)
      {
        if (checkOOB(Y + (3 - i) + dy - 1))
        {
          break;
        }
        if (curTetro.matrix[i][j] == 'X')
        {
          if (board[Y + i + dy - 1][X + j] != 0 || Y + i + dy > 20)
          {
            return true;
          }
        }
      }
    }
    return false;
  }
  bool collidesUp()
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (curTetro.matrix[i][j] == 'X')
        {
          if (Y + i < 1)
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  void tetroFall()
  {
    move(0, 1);
  }
  
  int bag[7] = {-1, -1, -1, -1, -1, -1, -1};
  void fillQueue()
  {
    for (int i = 0; i < 4; i++)
    {
      if (queue[i] < 0)
      {
        int r = -1;
        while (r == -1)
        {
          bool succ = true;
          int potR = random() % (7);
          for (int i = 0; i < 7; i++)
          {
            if (potR == bag[i])
            {
              succ = false;
            }
          }
          if (succ)
          {
            r = potR;
          }
        }
        bag[bagIndex] = r;
        handleBagIndex();
        queue[i] = r;
      }
    }
  }
  int bagIndex = 0;
  void handleBagIndex()
  {
    bagIndex += 1;
    if (bagIndex >= 7)
    {
      bagIndex = 0;
      clearBag();
    }
  }
  void clearBag()
  {
    for (int i = 0; i < 7; i++)
    {
      bag[i] = -1;
    }
  }
  void clearQueue()
  {
    for (int i = 0; i < sizeof(queue) / sizeof(int); i++)
    {
      queue[i] = -1;
    }
  }
  int updateQueue()
  {
    int next = queue[0];
    for (int i = 0; i < sizeof(queue) / sizeof(int); i++)
    {
      queue[i] = queue[i + 1];
    }
    queue[3] = -1;
    fillQueue();
    return next;
  }
  void getNewTetro()
  {
    spawnTetro(queue[0]);
    resetXY();
    updateQueue();
  }
  
  //  --- CLEARING LINES ---  //

  void checkLines()
  {
    int clears = 0;
    int lines[4] = {-1, -1, -1, -1};
    for (int i = 0; i < h; i++)
    {
      if (checkLine(i))
      {
        lines[clears] = i;
        clears += 1;
      }
    }
    if (clears > 0)
    {
      if (doAnim)
      {
        lineClearAnim(lines);
      }

      for (int i = 0; i < 4; i++)
      {
        if (lines[i] != -1)
        {
          clearLine(lines[i]);
        }
      }
      score += pointsPerLine[clears - 1] * mult;
      mult += 1;
    }
    else 
    {
      mult = 1;
    }
  }
  bool checkLine(int y)
  {
    for (int j = 1; j < w + 1; j++)
    {
      if (board[y][j] == 0)
      {
        return false;
      }
    }
    return true;
  }
  void clearLine(int y)
  {
    for (int i = y - 1; i > 0; i--)
    {
      if (i < h)
      {
        for (int j = 1; j < w + 1; j++)
        {
          board[i + 1][j] = board[i][j]; 
        }
      }
    }
  }
  void lineClearAnim(int lines[4])
  {
    doingAnim = true;
    int linesBuf[5][w];
    for (int c = 0; c < 4; c++)
    {
      for (int i = 0; i < 4; i++)
      {
        if (lines[i] != -1)
        {
          for (int j = 1; j < w + 1; j++)
          {
            if (c == 0)
            {
              linesBuf[i][j] = board[lines[i]][j];
            }
            board[lines[i]][j] = c % 2 == 0 ? 0 : linesBuf[i][j];
          }
        }
      }
      resetRaster();
      usleep(lineFlashMs * 1000);
      drawBoard();
      updateInfo();
      raster -> draw();
    }
    doingAnim = false;
  }

  //  --- TIME & LOGIC ---  //
  
  void waitFrame()
  { 
    usleep(ms * 1000);
  }
  void handleTime()
  {
    msPassed += ms;
    if (msPassed >= msToLvl + lvl * 1000)
    {
      lvlUp();
    }
  }
  void lvlUp()
  {
    msPassed = 0;
    if (lvl >= maxLvl)
    {
      return;
    }
    lvl += 1;
  }
  void countFrame()
  {
    frameCount += 1;

    if (frameCount >= fallFrames - lvl)
    {
      tetroFall();
      resetFrameCount();
    }
  }
  void resetFrameCount()
  {
    frameCount = 0;
  }

  //  --- RASTER ---  //
  
  int addY = 1;
  void updateInfo()
  {
    if (!lost)
    {
      raster -> addRString(12, 1, "<TETRIS>");
    }
    
    std::string lvlStr = lvl > 9 ? std::to_string(lvl) : "0" + std::to_string(lvl);
    raster -> addRString(12, 1 + addY, "LVL: " + lvlStr + " ");

    raster -> addVerLine(RasterLib::Pixel::Black, 12, 1, 20);
    raster -> addVerLine(RasterLib::Pixel::Black, 13, 1, 20);
    raster -> addVerLine(RasterLib::Pixel::Black, 14, 1, 20);
    raster -> addVerLine(RasterLib::Pixel::Black, 15, 1, 20);

    raster -> addRString(12, 3 + addY, "SCORE:");
    std::string scoreStr = std::to_string(score);
    while (scoreStr.length() < 8)
    {
      scoreStr += " ";
    }
    raster -> addRString(12, 4 + addY, scoreStr);

    raster -> addRString(12, 6 + addY, "HIGH: ");
    std::string highStr = std::to_string(high);
    while (highStr.length() < 8)
    {
      highStr += " ";
    }
    raster -> addRString(12, 7 + addY, highStr);
    
    raster -> addRString(12, 9 + addY, "-=<()>=-");
    

    raster -> addRString(12, 11 + addY, "NEXT: ");
    drawInfoTetro(Tetro(static_cast<Tetro::Shape>(queue[0])), 12, 12 + addY);
    raster -> addRString(12, 16 + addY, "HELD: ");
    if (held != -1)
    {
      drawInfoTetro(Tetro(static_cast<Tetro::Shape>(held)), 12, 17 + addY);
    }
  }
  void drawInfoTetro(Tetro t, int x, int y)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (t.matrix[i][j] == 'X')
        {
          raster -> setPixel(x + j, y + i, t.getColor());
        }
      }
    }
  }
  void clearBoard()
  {   
    for (int i = 0; i < h; i++)
    {
      for (int j = 1; j < w + 1; j++)
      {
        board[i][j] = 0;
      }
    }

  }
  void drawBoard()
  {
    for (int i = 0; i < h; i++)
    {
      for (int j = 1; j < w + 1; j++)
      {
        if (board[i][j] != 0)
        {
          raster -> setPixel(j, i + 1, static_cast<RasterLib::Pixel::Color>(board[i][j]));
        }
      }
    }
  }
  void resetRaster()
  {
    raster -> clear();
    raster -> addBorder(RasterLib::Pixel::Black);
    raster -> addVerLine(RasterLib::Pixel::Black, 11, 1, 20);
  }
};

#endif
