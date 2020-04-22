
#include "LedControl.h"
#include "binary.h"

int lightSensorPin = A0;

LedControl display = LedControl(12,11,10,4);

const uint64_t IMAGES[] = {
  0x1c2222222222221c,
  0x1c08080808080c08,
  0x3e0408102020221c,
  0x1c2220201820221c,
  0x20203e2224283020,
  0x1c2220201e02023e,
  0x1c2222221e02221c,
  0x040404081020203e,
  0x1c2222221c22221c,
  0x1c22203c2222221c,
  0x000000003f000000,
  0xc6c6e6f6decec600,
  0x7c66666666666600,
  0x7e06060606060600
  
};

void setup() {
  Serial.begin(9600);
  display.shutdown(0,false);
  display.setIntensity(0,5);
  display.clearDisplay(0); 
  display.shutdown(1,false);
  display.setIntensity(1,5);
  display.clearDisplay(1); 
  display.shutdown(2,false);
  display.setIntensity(2,5);
  display.clearDisplay(2); 
  display.shutdown(3,false);
  display.setIntensity(3,5);
  display.clearDisplay(3);

  pinMode(7, INPUT);
  
}

void displayImage(uint64_t image, int matrix) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(matrix, i, j, bitRead(row, j));
    }
  }
}

void setNumber(int c) {
    int ones = (c%10);
    int tens = ((c/10)%10);
    int hundreds = ((c/100)%10);
    int thousands = (c/1000);

    if(c == NULL){
      displayImage(IMAGES[13], 0);
      displayImage(IMAGES[13], 1);
      displayImage(IMAGES[12], 2);
      displayImage(IMAGES[11], 3); 
      return;
    }
    
    if(c < -999 && c > -10000){
      displayImage(IMAGES[9], 0);
      displayImage(IMAGES[9], 1);
      displayImage(IMAGES[9], 2);
      displayImage(IMAGES[10], 3); 
    }else if(c < -99 && c > -1000){
      displayImage(IMAGES[ones*(-1)], 0);
      displayImage(IMAGES[tens*(-1)], 1);
      displayImage(IMAGES[hundreds*(-1)], 2);
      displayImage(IMAGES[10], 3); 
    }else if(c < -9 && c > -100){
      displayImage(IMAGES[ones*(-1)], 0);
      displayImage(IMAGES[tens*(-1)], 1);
      displayImage(IMAGES[0], 2);
      displayImage(IMAGES[10], 3); 
    }else if(c < 0 && c > -10){
      displayImage(IMAGES[ones*(-1)], 0);
      displayImage(IMAGES[0], 1);
      displayImage(IMAGES[0], 2);
      displayImage(IMAGES[10], 3); 
    }else if(c < 10 && c > 0) {
      displayImage(IMAGES[ones], 0);
      displayImage(IMAGES[0], 1);
      displayImage(IMAGES[0], 2);
      displayImage(IMAGES[0], 3); 
  }else if(c < 100 && c > 9) {
      displayImage(IMAGES[ones], 0);
      displayImage(IMAGES[tens], 1);
      displayImage(IMAGES[0], 2);
      displayImage(IMAGES[0], 3); 
  }else if(c < 1000 && c > 99) {
      displayImage(IMAGES[ones], 0);
      displayImage(IMAGES[tens], 1);
      displayImage(IMAGES[hundreds], 2);
      displayImage(IMAGES[0], 3); 
  }else if(c < 10000 && c > 999) {
      displayImage(IMAGES[ones], 0);
      displayImage(IMAGES[tens], 1);
      displayImage(IMAGES[hundreds], 2);
      displayImage(IMAGES[thousands], 3); 
  } else if(c > 9999) {
          displayImage(IMAGES[9], 0);
      displayImage(IMAGES[9], 1);
      displayImage(IMAGES[9], 2);
      displayImage(IMAGES[9], 3); 
  } else {
      displayImage(IMAGES[0], 0);
      displayImage(IMAGES[0], 1);
      displayImage(IMAGES[0], 2);
      displayImage(IMAGES[0], 3); 
  }
}

void loop() {
  int analogValue = analogRead(lightSensorPin);
  if(analogValue <= 70){
display.shutdown(0,true);
display.shutdown(1,true);
display.shutdown(2,true);
display.shutdown(3,true);
  }else{
display.shutdown(0,false);
display.shutdown(1,false);
display.shutdown(2,false);
display.shutdown(3,false);
  }
  if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      if(data == "ERROR"){
        setNumber(NULL);
      }else{
        int count = data.toInt();
        setNumber(count);
      }
  }
  delay(200);
}
