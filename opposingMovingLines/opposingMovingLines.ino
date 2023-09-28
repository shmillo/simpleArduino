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

uint8_t lineOneCathode =  0b10000000;
uint8_t lineTwoCathode = 0b00000001;
uint8_t lineThreeCathode =  0b10000000;
uint8_t lineFourCathode = 0b00000001;
uint8_t lineFiveCathode =  0b00010000;
uint8_t lineSixCathode = 0b00000010;

uint8_t lineOneAnode =  0b00100000;
uint8_t lineTwoAnode = 0b00000001;
uint8_t lineThreeAnode =  0b00010000;
uint8_t lineFourAnode = 0b00000010;
uint8_t lineFiveAnode =  0b00001000;
uint8_t lineSixAnode = 0b10000000;

float moveTimeLineOne = 1.0 * 1e6; float lastMoveTimeLineOne = 0.0; int lineOneShiftBit = 0;
float moveTimeLineTwo = 0.5 * 1e6; float lastMoveTimeLineTwo = 0.0; int lineTwoShiftBit = 0;
float moveTimeLineThree = 0.2 * 1e6; float lastMoveTimeLineThree = 0.0; int lineThreeShiftBit = 0;
float moveTimeLineFour = 0.25 * 1e6; float lastMoveTimeLineFour = 0.0; int lineFourShiftBit = 0;
float moveTimeLineFive = 0.366 * 1e6; float lastMoveTimeLineFive = 0.0; int lineFiveShiftBit = 0;
float moveTimeLineSix = 0.4 * 1e6; float lastMoveTimeLineSix = 0.0; int lineSixShiftBit = 0;

float Time = 0.0;

void loop() 
{
  Time = micros();

  writeCells(~lineOneCathode, lineOneAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineTwoCathode, lineTwoAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineThreeCathode, lineThreeAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineFourCathode, lineFourAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineFiveCathode, lineFiveAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineSixCathode, lineSixAnode);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);


  if(Time - lastMoveTimeLineOne > moveTimeLineOne){
    lineOneShiftBit += 1;
    if(lineOneShiftBit > 8){
      lineOneShiftBit = 0;
    }
    lineOneCathode = 0b10000000 >> lineOneShiftBit;
    lastMoveTimeLineOne = Time;
  }

  if(Time - lastMoveTimeLineTwo > moveTimeLineTwo){
    lineTwoShiftBit += 1;
    if(lineTwoShiftBit > 8){
      lineTwoShiftBit = 0;
    }
    lineTwoCathode = 0b00000001 << lineTwoShiftBit;
    lastMoveTimeLineTwo = Time;
  }

  if(Time - lastMoveTimeLineThree > moveTimeLineThree){
    lineThreeShiftBit += 1;
    if(lineThreeShiftBit > 8){
      lineThreeShiftBit = 0;
    }
    lineThreeCathode = 0b10000000 >> lineThreeShiftBit;
    lastMoveTimeLineThree = Time;
  }

  if(Time - lastMoveTimeLineFour > moveTimeLineFour){
    lineFourShiftBit += 1;
    if(lineFourShiftBit > 8){
      lineFourShiftBit = 0;
    }
    lineFourCathode = 0b00000001 << lineFourShiftBit;
    lastMoveTimeLineFour = Time;
  }

  if(Time - lastMoveTimeLineFive > moveTimeLineFive){
    lineFiveShiftBit += 1;
    if(lineFiveShiftBit > 8){
      lineFiveShiftBit = 0;
    }
    lineFiveCathode = 0b10000000 >> lineFiveShiftBit;
    lastMoveTimeLineFive = Time;
  }

  if(Time - lastMoveTimeLineSix > moveTimeLineSix){
    lineSixShiftBit += 1;
    if(lineSixShiftBit > 8){
      lineSixShiftBit = 0;
    }
    lineSixCathode = 0b00000001 << lineSixShiftBit;
    lastMoveTimeLineSix = Time;
  }
}
