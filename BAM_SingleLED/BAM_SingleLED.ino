int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;

static int numLEDs = 8;
int brightnessMask[4] = {};
int cycle, currentLED, maskPosition, j = 0;
byte currentWriteCathode, currentWriteAnode = 0b00000000;

void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);

  ledBrightnessMask(0, 0);
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

void ledBrightnessMask(int ledNr, int brightness){
  brightness = constrain(brightness, 0, 15);
  for(int i = 3; i >= 0; i--){
    if(brightness - (1<<i) >= 0){
      brightness -= (1<<i);
      brightnessMask[(ledNr*4) + i] = 1;
    }     
    else{
      brightnessMask[(ledNr*4) + i] = 0;
    }
  }
}

void clearLeds(){
  writeCells(~0b00000000, 0b00000000);
}

void turnOnLed(){
 for(j = 0; j < 8; j++){
        currentWriteCathode = 0b00001000 & (0b00000001<<j);
			  writeCells(~currentWriteCathode, 0b00001000);
		}
}

int isIncreasing = 1;
int currentBrightness = 0; 
float lastBrightnessUpdate = 0.0; 
float brightnessUpdateInterval = 1.0e-2 * 1e6;
float Time = 0.0;

void loop() {
  Time = micros();

  if(Time - lastBrightnessUpdate > brightnessUpdateInterval){
    ledBrightnessMask(0, currentBrightness);
    if(isIncreasing){
      currentBrightness += 1;
      if(currentBrightness >= 15){
       currentBrightness = 15;
       isIncreasing = 0;
      }
    }
    else{
      currentBrightness -= 1;
      if(currentBrightness <= 0){
       currentBrightness = 0;
       isIncreasing = 1;
      }
    }
    lastBrightnessUpdate = Time;
  }

  for(cycle = 0; cycle < 16; cycle++){
      if (cycle == 1 && brightnessMask[0]) {
        turnOnLed();
      }
      else if ((cycle == 2 || cycle == 3) && brightnessMask[1]) {
        turnOnLed();
      }
      else if (cycle >= 4 && cycle <= 7 && brightnessMask[2]) {
        turnOnLed();
      }
      else if (cycle >= 8 && cycle <= 15 && brightnessMask[3]) {
        turnOnLed();
      }
      else{
        clearLeds();
      }
  }
}
