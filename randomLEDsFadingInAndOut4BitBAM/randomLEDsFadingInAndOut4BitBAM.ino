int dataPin = 8; 
int clockPin = 10;
int latchPin = 9;

int dataPin2 = 2;
int clockPin2 = 4;
int latchPin2 = 3;

int brightnessMask[32] = {};
int cycle, currentLED, maskPosition, j, i, k = 0;
byte currentWriteCathode = 0b00000000;
byte currentWriteAnode = 0b00000000;

byte cathode[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
byte anode[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
byte cathodeTemp[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
int shiftBit[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int holdValue[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int holdAmount = 10;
int isIncreasing[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
int currentBrightness[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
double writeInterval[8] = { 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 };
double lastMoveTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
double lastWriteTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
double finalWriteTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
double timeModifier[8] = { 0.9e-1 * 1.0e6, 0.5e-1 * 1e6, 0.01e-1 * 1e6, 0.1e-1 * 1e6, 0.4e-1 * 1e6, 0.3e-1 * 1e6, 0.1e-1 * 1e6, 0.2e-1 * 1e6 };
double writeIntervalInitializer = 2.91;
double finalWriteInterval[8] = { writeIntervalInitializer * timeModifier[0], writeIntervalInitializer * timeModifier[1], writeIntervalInitializer  * timeModifier[1], writeIntervalInitializer  * timeModifier[1], writeIntervalInitializer  * timeModifier[1], writeIntervalInitializer  * timeModifier[1], writeIntervalInitializer  * timeModifier[1], writeIntervalInitializer  * timeModifier[1] };
double Time = 0.0;

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

void turnOnLed(int rowNumber){
  currentWriteAnode = 0b00000001 << rowNumber;
  currentWriteCathode = cathodeTemp[rowNumber];
	writeCells(~currentWriteCathode, currentWriteAnode);
}

void loop(){

	Time = micros();
	for(i = 0; i < 8; i++){
		
    for(cycle = 0; cycle < 16; cycle++){

      int tempIndex = i*4;
      if (cycle == 1 && brightnessMask[tempIndex]) {
        turnOnLed(i);
      }
      else if ((cycle == 2 || cycle == 3) && brightnessMask[tempIndex + 1]) {
        turnOnLed(i);
      }
      else if (cycle >= 4 && cycle <= 7 && brightnessMask[tempIndex + 2]) {
        turnOnLed(i);
      }
      else if (cycle >= 8 && cycle <= 15 && brightnessMask[tempIndex + 3]) {
        turnOnLed(i);
      }
      else{
        clearLeds();
      }
    }

		if(Time - lastWriteTime[i] > finalWriteInterval[i]){
      	ledBrightnessMask(i, currentBrightness[i]);
      if(isIncreasing[i]){
          currentBrightness[i] += 1;
          if(currentBrightness[i] >= 15){
            currentBrightness[i] = 15;
            isIncreasing[i] = 0;
          }
      }
      else{
          currentBrightness[i] -= 1;
          if(currentBrightness[i] <= 0){
            currentBrightness[i] = 0;
            holdValue[i] += 1;
            if(holdValue[i] >= holdAmount){
              holdValue[i] = 0;
              isIncreasing[i] = 1;
              
              shiftBit[i] = random(0, 8);
              cathodeTemp[i] =  ( cathodeTemp[i] | (0b00000001<<(shiftBit[i]%8)) );
              
              shiftBit[i] = random(0, 8);
              cathodeTemp[i] =  ( cathodeTemp[i] & ~(0b00000001<<(shiftBit[i]%8)) );
    	        lastMoveTime[i] = Time;
              
              finalWriteInterval[i] = writeIntervalInitializer * ((random(10, 100.0)/400.0) * 1e6);
            }
          }
      }
      lastWriteTime[i] = Time; 
    }   
	}
} 