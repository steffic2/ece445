#include <Adafruit_MotorShield.h>
#include <FastLED.h>
#include <Servo.h>
#include <SPI.h>
#include <Adafruit_PCD8544.h>

Servo servo;
CRGB leds[8];
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepper = AFMS.getStepper(200, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  stepper->setSpeed(5);  // 5 rpm
  servo.attach(5);
  
  FastLED.addLeds<WS2812, 2, RGB>(leds, 8);
  for(int i = 0; i<8; i++){
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(31, INPUT_PULLUP);
}


int xVal;
int yVal;
int TOLERANCE = 10;
int theta = 45;
boolean searchFlag = false;

boolean checkXY(int* x, int* y){
  String moveData;
  int inter;
  int inter1;
  String xStr;
  String yStr;
  Serial.println(*x);
  if(Serial.available() > 0){
    moveData = Serial.readStringUntil('\n');
    inter = moveData.indexOf(',');
    inter1 = moveData.indexOf(',', inter+1);
    xStr = moveData.substring(0, inter);
    yStr = moveData.substring(inter+1, inter1);
    //sStr = moveData.substring(inter1);
    *x = xStr.toInt();
    *y = yStr.toInt();
    
    if(abs(*x)!= 1000){ //if difference between x and x threshold and y and y threshold is less than 10 stop molving
      return true;
    }
    else {
      return false;
    }
  }
  return false;
}

void wait(long i){
  long x = millis();
  while((millis() - x) < i){
    continue;
  }
}

void loop() {
  //This for loop controls the search logic. The outer most loop controls the carriage movement.
  //The inner two for loops controls the sweep of the arm; one for clockwise sweep the other for counterclockwise
  int theta=45;
   String xStr;
   String yStr;
   String sStr;
   int x;
   int y;
   int s;
   int inter;
   int inter1;
   String moveData;
   servo.write(map(theta, 0, 270, 0, 180));
  while(digitalRead(31) == 1){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  digitalWrite(LED_BUILTIN, LOW);
  wait(2000);

//  digitalWrite(LED_BUILTIN, LOW);
//  wait(100);
//  digitalWrite(LED_BUILTIN, HIGH);
//  wait(2000);
//  digitalWrite(LED_BUILTIN, LOW);
//  wait(10000);
//  digitalWrite(LED_BUILTIN, HIGH);
  
  for (int i = 0; i<6; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    if (theta<=45){
      for(int j = theta; j <= 225; j++){
        theta = j;
        servo.write(map(theta, 0, 270, 0, 180));
        wait(100);
//        Serial.println(theta);
        
        if(theta % 5 == 0){
//          Serial.println(theta);
          if(Serial.available() > 0){
          moveData = Serial.readStringUntil('\n');
          
          inter = moveData.indexOf(',');
          //inter1 = moveData.indexOf(',', inter+1);
          xStr = moveData.substring(0, inter);
          yStr = moveData.substring(inter+1);
          //sStr = moveData.substring(inter1+1);
          //Serial.println(xStr+" "+yStr+" "+sStr);
          x = xStr.toInt();
          y = yStr.toInt();
          //s = sStr.toInt();
          //Serial.println(x);
          if(abs(x) != 1000){
            Serial.println("Finished scanning for drone, Alignment in progress");
            searchFlag = true;
            break;
          }
          }
        }
      }
      digitalWrite(LED_BUILTIN, LOW);
    }
    
    else if (theta>=225){
      for(int j = theta; j>=45; j--){
        theta = j;
        servo.write(map(theta, 0, 270, 0, 180));
        wait(100);
//        Serial.println(theta);
        if(theta % 5 == 0){
//          Serial.println(theta);
          if(Serial.available() > 0){
          moveData = Serial.readStringUntil('\n');
          inter = moveData.indexOf(',');
          //inter1 = moveData.indexOf(',', inter+1);
          xStr = moveData.substring(0, inter);
          yStr = moveData.substring(inter+1);
          //sStr = moveData.substring(inter1+1);
          x = xStr.toInt();
          y = yStr.toInt();
          //s = sStr.toInt();
//          Serial.println(x);
          if(abs(x) != 1000){
            Serial.println("Finished scanning for drone, Alignment in progress");
            searchFlag = true;
            break;
          }
          }
        }
      }
      digitalWrite(LED_BUILTIN, LOW);
    }
    if(searchFlag){
      break;
    }
    stepper->step(65, BACKWARD, MICROSTEP);
  }
  
  while((abs(x) > TOLERANCE) | (abs(y) > TOLERANCE)){
    if(Serial.available() > 0){
          moveData = Serial.readStringUntil('\n');
          inter = moveData.indexOf(',');
          xStr = moveData.substring(0, inter);
          yStr = moveData.substring(inter+1);
          x = xStr.toInt();
          y = yStr.toInt();
    }
    if(x < TOLERANCE){
      stepper->step(5, BACKWARD, MICROSTEP);
    }
    if(x > -TOLERANCE){
      stepper->step(5, FORWARD, MICROSTEP);
    }
    if(y < TOLERANCE){
      theta++;
      servo.write(map(theta, 0, 270, 0, 180));
    }
    if(y > -TOLERANCE){
      theta--;
      servo.write(map(theta, 0, 270, 0, 180));
    }
    //checkXY(&xVal, &yVal);
  }
  Serial.println("Alignment complete");
  while(digitalRead(31) == 1){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  //contrast is 0x32
  
}
