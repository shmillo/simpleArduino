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

float moveTimeLineOne = 1.0 * 1e6; float lastMoveTimeLineOne = 0.0; int lineOneShiftBit = 0;
float moveTimeLineTwo = 0.5 * 1e6; float lastMoveTimeLineTwo = 0.0; int lineTwoShiftBit = 0;
float Time = 0.0;

void loop() 
{
  Time = micros();

  writeCells(~lineOneCathode, 0b00100000);  
  delay(1);
  writeCells(~0b00000000, 0b00000000);

  writeCells(~lineTwoCathode, 0b00000001);  
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
}
