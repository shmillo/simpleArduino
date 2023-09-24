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

float sCurveTime(float brightness)
{
  //0 - 255 input, 0.0-1.0 output
  return (1.0/(1.0+exp(((brightness/21.0)-6.0) * -1.0))*255.0/255.0);
}

uint8_t cathode = 0b00100000;

float Time = 0.0; float lastWriteTime = 0.0; float timeModifier = 1.9e-2 * 1e6;
float writeInterval = 255.0; float finalWriteInterval = sCurveTime(writeInterval) * timeModifier; 
int isIncreasing = 1;

void loop() 
{
  Time = micros();
  if(Time - lastWriteTime > finalWriteInterval)
  {
    writeCells(~cathode, 0b00001000); 

    lastWriteTime = Time;
    if(isIncreasing == 1)
    {
      writeInterval -= 1.0;
      if(writeInterval < 0.0)
      {
        writeInterval = 0.0;
        isIncreasing = 0;
      }
      finalWriteInterval = sCurveTime(writeInterval) * timeModifier;
    }
    else
    {
      writeInterval += 1;
      if(writeInterval >= 255.0)
      {
        writeInterval = 255.0;
        isIncreasing = 1;
      }
      finalWriteInterval = sCurveTime(writeInterval) * timeModifier;
    }
   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }
}

