int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;
int clrPin = 11;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;
int clrPin2 = 5;

void setup() 
{
  randomSeed(100);
  
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

int row = 0; int column = 0;

byte currentOutAnode = 0b00000001;
  
bool isDecreasing = false;

uint32_t lastRefreshTime = 0;
float RefreshSpeed = 20;

uint32_t lastDutyCycleRefreshTime = 0;
float dutyCycleRefreshSpeed = RefreshSpeed*20000;

void initColumnMatrix()
{
  for (int i = 0; i < 8; i+=1)
  {
    byte tempByte = 0b00000000;
    for(int j = 0; j < 8; j+=1)
    {
        tempByte |= (mat[i][j]<<j);
    }
    currentOutCathode[i] = tempByte;
  }
} 

void clearColumnMatrix()
{
    for(int j = 0; j < 8; j+=1)
    {
        currentOutCathode[j] = 0b00000000;
    }
}

void loop() 
{
  uint32_t Time = micros();

  if( Time - lastDutyCycleRefreshTime > (dutyCycleRefreshSpeed) )
  {
    clearColumnMatrix(); rotate90(); initColumnMatrix();
    lastDutyCycleRefreshTime = Time;
  }
  
  if( Time - lastRefreshTime > (RefreshSpeed) )
  {
      byte currentBit = (currentOutCathode[column] >> row) & 0b00000001;
      writeCells(~(currentBit<<row), (currentOutAnode<<column) );  
      lastRefreshTime = Time;  

      if(row < 7){ row += 1; }
      else if(row >= 7)
      { 
          column += 1;
          if(column >= 7) { column = 0; }  
          row = 0; 
      }
  }
  else
  {
      writeCells(0b11111111, 0b00000000);  
  }
}
