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

float gammaCorrectedCurve(float brightness)
{
  return (pow(brightness/255.0, 2.5) * 255.0 + 0.5)/255.0;
}

byte cathode[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
byte cathodeTemp[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
int shiftBit[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int isIncreasing[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
float writeInterval[8] = { 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 };
float lastMoveTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
float lastWriteTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
float finalWriteTime[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
float timeModifier[8] = { 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6, 0.1e-1 * 1e6 };
float writeIntervalInitializer = gammaCorrectedCurve(255.0) * timeModifier[0];
float finalWriteInterval[8] = { writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer, writeIntervalInitializer };
float moveTime[8] = { 0.05 * 1e6, 0.45 * 1e6, 0.65 * 1e6, 0.25 * 1e6, 0.15 * 1e6, 0.85 * 1e6, 0.1 * 1e6, 0.5 * 1e6 };
int i, j, k = 0;
float Time = 0.0;
byte currentWriteAnode = 0b00000000;
byte currentWriteCathode = 0b00000000;

void loop(){

	Time = micros();

	for(i = 0; i < 8; i++){
		currentWriteAnode = 0b00000001 << i;	
		for(j = 0; j < 8; j++){
        currentWriteCathode = cathode[i] & (0b0000001<<j);
			  writeCells(~currentWriteCathode, currentWriteAnode);
		}
    writeCells(~0b00000000, 0b00000000);
	}

	for(i = 0; i < 8; i++){
		if(Time - lastMoveTime[i] > moveTime[i]){

    			if(shiftBit[i] < 8){
      				cathodeTemp[i] =  ( cathodeTemp[i] | (0b00000001<<(shiftBit[i]%8)) );
    			}
    			else if(shiftBit[i] >= 8){
      				cathodeTemp[i] =  ( cathodeTemp[i] & ~(0b00000001<<(shiftBit[i]%8)) );
    			}
    
    			shiftBit[i] += 1;
    			if(shiftBit[i] > 16){
      				shiftBit[i] = 0;
    			} 
    			lastMoveTime[i] = Time;
		}
		
		if(Time - lastWriteTime[i] > finalWriteInterval[i]){
			if(isIncreasing[i] == 1){
      				writeInterval[i] -= 1.0;
      				if(writeInterval[i] < 0.0){
        				writeInterval[i] = 0.0;
        				isIncreasing[i] = 0;
      				}
      				finalWriteInterval[i] = gammaCorrectedCurve(writeInterval[i]) * timeModifier[i];
    			}
    			else{
      				writeInterval[i] += 1.0;
      				if(writeInterval[i] >= 255.0){
        				writeInterval[i] = 255.0;
        				isIncreasing[i] = 1;
      				}
      				finalWriteInterval[i] = gammaCorrectedCurve(writeInterval[i]) * timeModifier[i];
    			}
			lastWriteTime[i] = Time;
			cathode[i] = cathodeTemp[i];
  	}
		else{
			cathode[i] = 0b00000000;
		}
	}
} 
