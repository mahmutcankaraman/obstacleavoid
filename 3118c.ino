#include <SoftwareSerial.h>   //Include libraries: SoftwareSerial
#include <Wire.h>           //Include libraries: wire
#include <LiquidCrystal.h>
#include <DS3231.h>
    

#define PIN 48
   int seviye_ses; //Sesin şiddetini seviye değişkeni olarak tanımladık.
// Init the DS3231 using the hardware interface
DS3231  rtc(6, 7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


const int LED=13;
const int Sensorsound = A1; //A1 girişini analog giriş olarak belirledik
const int esik_ses = 74; // ses şiddetinin eşik değerini buradan belirleriz.
int esikDegeriGas = 300;               //Gaz eşik değerini belirliyoruz.    
int buzzerPin = 9;                  //Buzzerın takılacağı pin
const int LeftMotorForward = 32;
const int LeftMotorBackward = 30;
const int RightMotorForward = 34;
const int RightMotorBackward = 36; //bunlar değişik olabilir sıra olarak tam emin değilim
const int trigPin = 40;
const int echoPin = 41;


#include <Servo.h>
Servo servo_motor; 


void setup(){
  servo_motor.attach(22);
  pinMode(LeftMotorForward,OUTPUT);
  pinMode(RightMotorForward,OUTPUT);
  pinMode(LeftMotorBackward,OUTPUT);
  pinMode(RightMotorBackward,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(buzzerPin, OUTPUT);       //Buzzer pinimizi çıkış olarak ayarlıyoruz.
  pinMode(A0,INPUT);
  Serial.begin(9600);
  
  
  lcd.begin(16, 2);

 
  
  
  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(14, 02, 06);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(22, 06, 2023);   // Set the date to January 1st, 2014
}

void loop(){ 

gassensor();
engeldenkac();
soundsensor();

 
 

 }





void engeldenkac(){
    servo_motor.write(60); //our servo pin
    delay(200);
int distance ;
  digitalWrite (trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration;
  // Measure the response from the HC-SR04 Echo Pin
  duration = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance= duration*0.034/2;
  Serial.println(distance);
 
  if (distance<=27){
    moveStop();
    moveBackward();
    delay(200);
     moveStop();
   servo_motor.write(0);
  delay(200);
  int distance1 ;
  digitalWrite (trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration1;
  // Measure the response from the HC-SR04 Echo Pin
  duration1 = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance1= duration1*0.034/2;
  Serial.print("Mesafe1=");
    Serial.println(distance1);
    delay(200);
    servo_motor.write(110);
  delay(300);
  int distance2;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
  int duration2;
  duration2 = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance2= duration2*0.034/2;
    Serial.print("Mesafe2=");
    Serial.println(distance2);

    servo_motor.write(60);
 
  if (distance1>=distance2){
    turnLeft();
    delay(100);

  }
  else{
    turnRight();
   delay(100);
  }
  delay(350);
  }
  else {
    rtcclock();
moveForward();
delay(380);
moveStop();

  }

}
void soundsensor(){
  
  seviye_ses = analogRead(Sensorsound);
  Serial.println(seviye_ses);
  if(seviye_ses > esik_ses) // eğer ses seviyesi eşik değerinden büyük ise Led yanar ve bir saniye sonra söner.
   {
    digitalWrite (LED,HIGH);
    delay (1000);
    digitalWrite(LED,LOW);
  }
  else
  {
    digitalWrite(LED,LOW); //eğer ses seviyesi eşik değerden küçük ise led söner
    }
}

void gassensor(){
  int degergas;                          //Sensörden okunan değer
  degergas = analogRead(A0);
  Serial.println(degergas);
  if(degergas > esikDegeriGas){           //Sensörden okunan değer eşik değerinden büyükse çalışır.
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  else{                             //Sensörden okunan değer eşik değerinin altındaysa çalışır.
    digitalWrite(buzzerPin, LOW);
  }

}



void rtcclock(){
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  lcd.setCursor(2, 0);
  lcd.print(rtc.getDateStr());

  // Send time
  Serial.println(rtc.getTimeStr());
   lcd.setCursor(2, 1);
  lcd.print(rtc.getTimeStr());
  // Wait one second before repeating :)
  delay (1000);

}

int lookRight(){  
  servo_motor.write(0);
  delay(500);
  int distance1 ;
  digitalWrite (trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration1;
  // Measure the response from the HC-SR04 Echo Pin
  duration1 = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance1= duration1*0.034/2;
  Serial.print("Mesafe1=");
    Serial.println(distance1);
  
  
  
}

int lookLeft(){
  servo_motor.write(100);
  delay(500);
  int distance2;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
  int duration2;
  duration2 = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance2= duration2*0.034/2;
    Serial.print("Mesafe2=");
    Serial.println(distance2);
  delay(100);
  

}



void moveStop(){
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveBackward(){

  

    
    
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  
}

void moveForward(){

 

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
}

void turnRight(){

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(1250);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
 
  
  
}

void turnLeft(){

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(1250);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

