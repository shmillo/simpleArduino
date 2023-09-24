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

float gammaCorrectedCurve(float brightness)
{
  return (pow(brightness / 255.0, 2.5) * 255.0 + 0.5)/255.0;
}

float Time = 0.0; 
float lastWriteTime = 0.0; float lastWriteTime2 = 0.0; 
float lastWriteTime3 = 0.0; float lastWriteTime4 = 0.0; 

float writeInterval = (gammaCorrectedCurve(255.0*0.05) * 1.0e4); float writeInterval2 = (gammaCorrectedCurve(255.0*0.35) * 1.0e4); 
float writeInterval3 = (gammaCorrectedCurve(255.0*0.65) * 1.0e4); float writeInterval4 = (gammaCorrectedCurve(255.0) * 3.0e4); 

int cathodeBrightnessOne = 0b00000000;
int cathodeBrightnessTwo = 0b00000000;
int cathodeBrightnessThree = 0b00000000;
int cathodeBrightnessFour = 0b00000000;

int valueMaskOne = 0b00000011;
int valueMaskTwo = 0b00001100;
int valueMaskThree = 0b00110000;
int valueMaskFour = 0b11000000;

int currentCathodeValue = 0b00000001;
int maskResult = 0b00000000;
int shiftValue = 0;

float cathodeShiftInterval = 0.25 * 1e6;
float lastCathodeShiftTime = 0.0;

void loop() 
{

  currentCathodeValue = 0b00000001 << shiftValue;

  Time = micros();
  if(Time - lastWriteTime > writeInterval)
  {
    maskResult = (currentCathodeValue & valueMaskOne);
    writeCells(~maskResult, 0b00010000); 
    lastWriteTime = Time;   
  }

  if(Time - lastWriteTime2 > writeInterval2)
  {
    maskResult = (currentCathodeValue & valueMaskTwo);
    writeCells(~maskResult, 0b00010000); 
    lastWriteTime2 = Time;   
  }

  if(Time - lastWriteTime3 > writeInterval3)
  {
    maskResult = (currentCathodeValue & valueMaskThree);
    writeCells(~maskResult, 0b00010000); 
    lastWriteTime3 = Time;   
  }

  if(Time - lastWriteTime4 > writeInterval4)
  {
    maskResult = (currentCathodeValue & valueMaskFour);
    writeCells(~maskResult, 0b00010000); 
    lastWriteTime4 = Time;   
  }

  if(Time - lastCathodeShiftTime > cathodeShiftInterval)
  {
    shiftValue += 1;
    if(shiftValue > 7)
    {
      shiftValue = 0;
      currentCathodeValue = 0b00000001;
    }
    lastCathodeShiftTime = Time;
  }

}