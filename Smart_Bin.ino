/*  
 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND
*/

#include "HX711.h"
#include <LiquidCrystal.h>

#define DOUT  7 //HX711 Chips interfacing Yellow Wire
#define CLK  8 //HX711 Chips interfacing Purple Wire 
#define buzzer 13 //Buzzer Trigger
#define trigPin 10  //ultrasonic sensor
#define echoPin 9   //ulrasonic sensor
#define blueLED A0  //bin emptying LED
#define calibration_factor 218.0  //this is the calabration factor for the scale

bool alertTone = false;
long distance;
int Contrast=75;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
HX711 scale;


void setup() {
  Serial.begin(9600);
  analogWrite(6,Contrast);
  lcd.begin(16,2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(A0, OUTPUT);
  
  scale.begin(DOUT, CLK);

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
}

void loop() {
  //You can change this to kg but you'll need to refactor the calibration_factor
  distance = getDistance();
  
  if(distance<33 && alertTone == false || alertTone == true){
    delay(5000);
    distance = getDistance();
    if(distance<33 && alertTone == false){
      lcd.setCursor(0, 0);
      lcd.print("Change BIN       ");
      printWeight();
      alertTone = true;
      digitalWrite(A0, HIGH);
    }else if(distance<33 && alertTone == true){
      lcd.setCursor(0, 0);
      lcd.print("Change BIN       ");
      printWeight();
    }
  }else{
    printDistance();
    printWeight();
  }
  
}


long getDistance(){
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void printWeight(){
  lcd.setCursor(0, 1);
  lcd.print("Weight: "); // Prints "Arduino" on the LCD
  lcd.print(scale.get_units(1), 1); //scale.get_units() returns a float
  lcd.print(" g");
}

void trigBuzzer(){
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer,LOW);
}

void printDistance(){
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");
}
