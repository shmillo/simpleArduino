int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;

void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
}

void writeCells(byte row, byte col)
{ 
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, row);
    digitalWrite(latchPin, HIGH);

    digitalWrite(latchPin2, LOW);
    shiftOut(dataPin2, clockPin2, LSBFIRST, col);
    digitalWrite(latchPin2, HIGH);  
}

uint8_t cathode = 0b00100000;

float Time = 0.0; float lastWriteTime = 0.0; 
float lowerThreshold = (1e-5 * 1e6); float upperThreshold = (1e-2 * 1e6); float writeInterval = upperThreshold; 
int isIncreasing = 1;

void loop() 
{
  Time = micros();
  if(Time - lastWriteTime > writeInterval)
  {
    writeCells(~cathode, 0b00001000); 

    lastWriteTime = Time;
    if(isIncreasing == 1)
    {
      writeInterval *= 0.998;
      if(writeInterval < lowerThreshold)
      {
        isIncreasing = 0;
      }

    }
    else
    {
      writeInterval *= 1.00912;
      if(writeInterval >= upperThreshold)
      {
        isIncreasing = 1;
      }
    }
   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }
}

