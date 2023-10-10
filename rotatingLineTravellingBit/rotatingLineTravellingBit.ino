int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;
int clrPin = 11;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;
int clrPin2 = 5;

int row, i, j = 0; int column = 0;
byte currentOutAnode = 0b00000001;
uint32_t lastRefreshTime = 0;
float RefreshSpeed = 40000;

float lastClearTime = 0;
float clearSpeed = RefreshSpeed * 0.5;

byte currentWriteCathode = 0b00000000;
int stage = 0;
int outerBoundRow = 5; int innerBoundRow = 2;
int outerBoundCol = 5; int innerBoundCol = 2;


void setup() 
{

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clrPin, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(clrPin2, OUTPUT);

  initColumnMatrix();
}

int mat[8][8] = 
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

byte currentOutCathode[8] = 
{
   0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000
};

void rotate90()
{
    int N = 8; 
    for(int x = 0; x < 4; x++)
    {
        for(int y = x; y < N - x - 1; y++)
        {
            int temp = mat[x][y];
            mat[x][y] = mat[y][N - 1 - x];
            mat[y][N - 1 - x] = mat[N - 1 - x][N - 1 - y];
            mat[N - 1 - x][N - 1 - y] = mat[N - 1 - y][x];
            mat[N - 1 - y][x] = temp;
        }
    }
}

void clearCells()
{
    digitalWrite(clrPin, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ~0b00000000);
    digitalWrite(latchPin, HIGH);
  
    digitalWrite(clrPin2, HIGH);
    digitalWrite(latchPin2, LOW);
    shiftOut(dataPin2, clockPin2, MSBFIRST, 0b00000000);
    digitalWrite(latchPin2, HIGH);  
}

void writeCells(byte row, byte col)
{ 
    digitalWrite(clrPin, HIGH);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, row);
    digitalWrite(latchPin, HIGH);

    digitalWrite(clrPin2, HIGH);
    digitalWrite(latchPin2, LOW);
    shiftOut(dataPin2, clockPin2, LSBFIRST, col);
    digitalWrite(latchPin2, HIGH);  
}

void initColumnMatrix()
{
  for (i = 0; i < 8; i+=1)
  {
    byte tempByte = 0b00000000;
    for(j = 0; j < 8; j+=1)
    {
        tempByte |= (mat[i][j]<<j);
    }
    currentOutCathode[i] = tempByte;
  }
} 

void clearColumnMatrix()
{
  for(j = 0; j < 8; j+=1)
    {
      currentOutCathode[j] = 0b00000000;
    }
}

void loop() 
{
  uint32_t Time = micros();

  if( Time - lastRefreshTime > RefreshSpeed )
  {
    byte currentBit = ((currentOutCathode[column] >> row) & 0b00000001);
    writeCells(~(currentBit<<row), (currentOutAnode<<column) );  
    lastRefreshTime = Time;  

    if(stage == 0){
      if(row <= outerBoundRow){ row += 1; }
      else if(row > outerBoundRow)
      { 
        column += 1;
        row = innerBoundRow;
        if(column > outerBoundCol) { 
          column = innerBoundCol; 
          row = innerBoundRow;
          stage = 1;
          clearColumnMatrix(); rotate90(); initColumnMatrix();
        }  
      }
    }
    else if(stage == 1){
      if(column <= outerBoundCol){ column += 1; }
      else if(column > outerBoundCol)
      { 
        row += 1;
        column = innerBoundCol; 
        if(row > outerBoundRow) { 
          row = outerBoundRow; 
          column = outerBoundCol;
          stage = 2;
          clearColumnMatrix(); rotate90(); initColumnMatrix();
        }  
      }
    }
    else if(stage == 2){
      if(row >= innerBoundRow){ row -= 1; }
      else if(row < innerBoundRow)
      { 
        row = outerBoundRow; 
        if(column >= innerBoundCol){ column -= 1; }
        else if(column < innerBoundCol) { 
          row = outerBoundRow; 
          column = innerBoundCol;
          stage = 3;
          clearColumnMatrix(); rotate90(); initColumnMatrix();
        }  
      }
    }
    else if(stage == 3){
      if(column <= outerBoundCol){ column += 1; }
      else if(column > outerBoundCol)
      { 
        column = innerBoundCol;
        if(row >= innerBoundRow){ row -= 1; }
        else if(row < innerBoundRow) { 
          row = innerBoundRow; 
          column = innerBoundCol;
          stage = 0;
          clearColumnMatrix(); rotate90(); initColumnMatrix();
        }  
      }
    }
  }
  else{
    if(Time - lastClearTime > clearSpeed){
      writeCells(~0b00000000, 0b00000000);
      lastClearTime = Time;
    }
  }
}
