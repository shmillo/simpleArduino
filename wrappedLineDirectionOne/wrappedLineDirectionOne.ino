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

byte lineTwoCathode = 0b00000000;
byte lineTwoCathodeTemp = 0b00000000;
byte lineTwoAnode = 0b00001000;
float moveTimeLineTwo = 0.5 * 1e6; float lastMoveTimeLineTwo = 0.0; int lineTwoShiftBit = 0;

float Time = 0.0;
int i = 0;

void loop() 
{
  Time = micros();

  for(i = 0; i < 8; i++){
    lineTwoCathode = lineTwoCathodeTemp & (0b00000001<<i);
    writeCells(~lineTwoCathode, lineTwoAnode);  
    delay(1);
    writeCells(~0b00000000, 0b00000000);
  }

  if(Time - lastMoveTimeLineTwo > moveTimeLineTwo){

    if(lineTwoShiftBit < 8){
      lineTwoCathodeTemp =  ( lineTwoCathodeTemp | (0b00000001<<(lineTwoShiftBit%8)) );
    }
    else if(lineTwoShiftBit >= 8){
      lineTwoCathodeTemp =  ( lineTwoCathodeTemp & ~(0b00000001<<(lineTwoShiftBit%8)) );
    }
    
    lineTwoShiftBit += 1;
    if(lineTwoShiftBit > 16){
      lineTwoShiftBit = 0;
    } 
    lastMoveTimeLineTwo = Time;
  }

}