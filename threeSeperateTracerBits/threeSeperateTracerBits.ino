int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;
int clrPin = 11;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;
int clrPin2 = 5;

int row, i = 0; int column = 0;
int j = 12; int h = 18;
uint32_t lastRefreshTime = 0;
float RefreshSpeed = 50000;

uint32_t lastRefreshTimeTwo = 0;
float RefreshSpeedTwo = RefreshSpeed * 0.666 * 2.0;

uint32_t lastRefreshTimeThree = 0;
float RefreshSpeedThree = RefreshSpeed * 0.375 * 2.0;

float lastClearTime = 0;
float clearSpeed = RefreshSpeed * 0.5;

byte currentWriteCathode = 0b00000000;

byte currentOutCathode[16] = 
{
  0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100
};

byte currentOutAnode[16] = 
{
  0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00010000, 0b00001000, 0b00000100
};



byte currentOutCathodeTwo[24] = 
{
  0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b00100000, 0b00010000, 0b00010000, 0b00000100, 0b00000010
};

byte currentOutAnodeTwo[24] = 
{
  0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010
};



byte currentOutCathodeThree[32] = 
{
  0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b01000000,
  0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001
};

byte currentOutAnodeThree[32] = 
{
  0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001
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

void loop() 
{
  uint32_t Time = micros();

  if( Time - lastRefreshTime > RefreshSpeed )
  {
    for(row = 0; row < 8; row++){
      for(column = 0; column < 8; column++){
        writeCells(~currentOutCathode[i], currentOutAnode[i] );  
      }
    }
    writeCells(~0b00000000, 0b00000000);
    i += 1; if(i >= 16){ i = 0; }
    lastRefreshTime = Time;  
  }
  else{
    if(Time - lastClearTime > clearSpeed){
      writeCells(~0b00000000, 0b00000000);
      lastClearTime = Time;
    }
  }

  if( Time - lastRefreshTimeTwo > RefreshSpeedTwo )
  {
    for(row = 0; row < 8; row++){
      for(column = 0; column < 8; column++){
        writeCells(~currentOutCathodeTwo[j], currentOutAnodeTwo[j] );  
      }
    }
    writeCells(~0b00000000, 0b00000000);
    j += 1; if(j >= 24){ j = 0; }
    lastRefreshTimeTwo = Time;  
  }

  if( Time - lastRefreshTimeThree > RefreshSpeedThree )
  {
    for(row = 0; row < 8; row++){
      for(column = 0; column < 8; column++){
        writeCells(~currentOutCathodeThree[h], currentOutAnodeThree[h] );  
      }
    }
    writeCells(~0b00000000, 0b00000000);
    h += 1; if(h >= 32){ h = 0; }
    lastRefreshTimeThree = Time;  
  }

}
