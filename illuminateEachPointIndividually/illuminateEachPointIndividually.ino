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

uint8_t col = 0;
int row = 0;
int shiftVal = 0;
int timer = 0;

uint8_t cathode = 0b00000001;
void loop() 
{
  writeCells(~(cathode << shiftVal), 0b00000001 << row);  
  delay(1); 
  timer += 1;
  
  if(col < 7){ col += 1; }
  else if(col >= 7)
  { 
    col = 0; 
    if(timer >= 400)
    {
      timer = 0;
      if(row < 7){ row += 1; }
      else if(row >= 7)
      { 
        row = 0; 
        shiftVal += 1;
        if(shiftVal >= 7)
        {
          shiftVal = 0;
        }
        
      }
    }
  }
}

