#include <Wire.h>
#define LEN(x) (sizeof(x)/sizeof(x[0]))

#define ADDRESS 0xD0

int n = LOW;

typedef struct {
  int a;
  int b;
  int c;
} Message;

typedef struct {
  int pinA;
  int pinB;
  int pos;
  int pinALast;
} Encoder;

Encoder pins[] = {
  {3, 2, 0, LOW},
  {6, 5, 0, LOW},
  {9, 8, 0, LOW}
};

Message msg;

void initPins() {
  for (int i = 0; i < LEN(pins); i++) {
   pinMode (pins[i].pinA,INPUT);
   pinMode (pins[i].pinB,INPUT);
  }
}

void readPins() {
  for (int i = 0; i < LEN(pins); i++) {
    //position
    n = digitalRead(pins[i].pinA);
    if ((pins[i].pinALast == LOW) && (n == HIGH)) {
     if (digitalRead(pins[i].pinB) == LOW) {
       pins[i].pos--;
     } else {
       pins[i].pos++;
     }
     
     printPinValues();
    } 
    
    pins[i].pinALast = n;
  }
}


void resetPinValues() {
  for (int i = 0; i < LEN(pins); i++) {
    pins[i].pos = 0;
  }
}

void printPinValues() {
  Serial.print("[");
  
  for (int i = 0; i < LEN(pins); i++) {
    Serial.print(i, DEC); Serial.print(": ");
    Serial.print(pins[i].pos, DEC); Serial.print(" \t");
  }
  
  Serial.println("]");
}

Message getMessage() {
  msg.a = pins[0].pos;
  msg.b = pins[1].pos;
  msg.c = pins[2].pos;
  resetPinValues();
  return msg;
}

void requestEvent() {
  Message msg = getMessage();
  Wire.write((char*)(&msg), sizeof(msg)); 
  Serial.println("Response sent.");
}

void setup() { 
  Wire.begin(ADDRESS);                
  Wire.onRequest(requestEvent);
  initPins();
  Serial.begin (9600);
  Serial.println ("encoder example.");
} 

void loop() { 
  readPins();  
} 
 
