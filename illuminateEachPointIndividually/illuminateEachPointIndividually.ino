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

uint8_t cathode = 0b00000001;
void loop() 
{
  writeCells(~(cathode << shiftVal), 0b00000001 << row);  
  delay(1); 
}

