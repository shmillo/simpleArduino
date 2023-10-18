int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;
int clrPin = 11;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;
int clrPin2 = 5;

int i, column, row, wrapCount = 0;
int j = 0; int k = 0; int h = 0;

int locationArray[4] = { 0, 0, 0, 0 };
int locationTempArray[4] = { 0, -2, -4, -8 };

byte currentWriteCathode, currentWriteAnode = 0b00000000;

int locX[64] = {
  0, 0, 1, 1, 2, 3, 3, 2, 2, 3, 3, 2, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 2, 
  2, 3, 3, 3, 2, 2, 3, 4, 5, 5, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 6, 6, 7, 7, 7,
  6, 6, 5, 4, 4, 5, 5, 4, 4, 5, 6, 6, 7, 7
};

int locY[64] = {
  0, 1, 1, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 2, 2, 3, 4, 4, 5, 5, 6, 7, 7, 6, 6, 
  7, 7, 6, 5, 5, 4, 4, 4, 4, 5, 5, 6, 7, 7, 6, 6, 7, 7, 6, 5, 5, 4, 4, 3, 2,
  2, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 0
};

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

byte currentBit = 0b00000000;
uint32_t lastRefreshTime = 0;
float RefreshSpeed = 70000;

float lastClearTime = 0;
float clearSpeed = RefreshSpeed * 0.25;

void loop() 
{
  uint32_t Time = micros();

  for(i = 0; i < 4; i++){
    writeCells(~0b00000000, 0b00000000); 
    writeCells(~(0b00000001 << locX[locationArray[i]]), (0b00000001 << locY[locationArray[i]])  );
  }

  if( Time - lastRefreshTime > RefreshSpeed )
  {
    for(i = 0; i < 4; i++){
      locationTempArray[i] += 1;
      locationTempArray[i] %= (63 + (2*i));
      if(locationTempArray[i] >= 0){
        locationArray[i] = locationTempArray[i];
      }
    }
    lastRefreshTime = Time;  
  }

}
