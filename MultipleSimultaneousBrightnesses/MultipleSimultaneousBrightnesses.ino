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


float Time = 0.0; 
float lastWriteTime = 0.0; float lastWriteTime2 = 0.0; 
float lastWriteTime3 = 0.0; float lastWriteTime4 = 0.0; 

float writeInterval = (3.42e-2 * 1e6); float writeInterval2 = (.8e-2 * 1e6); 
float writeInterval3 = (1.1e-3 * 1e6); float writeInterval4 = (1e-10 * 1e6); 


int cathodeBrightnessOne = 0b00000000;
int cathodeBrightnessTwo = 0b00000000;
int cathodeBrightnessThree = 0b00000000;
int cathodeBrightnessFour = 0b00000000;

void loop() 
{

  Time = micros();
  if(Time - lastWriteTime > writeInterval)
  {
    writeCells(~0b00000001, 0b00010000); 
    lastWriteTime = Time;   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }

  if(Time - lastWriteTime2 > writeInterval2)
  {
    writeCells(~0b00000100, 0b00010000); 
    lastWriteTime2 = Time;   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }

  if(Time - lastWriteTime3 > writeInterval3)
  {
    writeCells(~0b00010000, 0b00010000); 
    lastWriteTime3 = Time;   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }

  if(Time - lastWriteTime4 > writeInterval4)
  {
    writeCells(~0b01000000, 0b00010000); 
    lastWriteTime4 = Time;   
  }
  else
  {
    writeCells(~0b00000000, 0b00000000);
  }

}

