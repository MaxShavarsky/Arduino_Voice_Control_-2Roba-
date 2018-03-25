const int pinPWMA = 6; // живлення для моторчика №2        //
const int pinAIN2 = 7;                                      //
const int pinAIN1 = 8;                                      //
const int pinBIN1 = 9;                                      //
const int pinBIN2 = 10;                                     //
const int pinPWMB = 11;  // живлення для моторчика №2      //
const int pinSTBY = 12; // пін дозволу руху                 //
                                                            //
const int waitTime = 2000;   //затримка                     //
const int speed = 200;      //швидкість                     //
                                                            //
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };     // Колеса
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };     //
enum moveDirection {                                        //
   forward,                                                 //
   backward                                                 //
};                                                          //
enum turnDirection {                                        //
   clockwise,                                               //
   counterClockwise                                         //
};                                                          //
                                                            //

#include <SoftwareSerial.h>                                 //
#include <DFPlayer_Mini_Mp3.h>                              // мп3 плеєр
SoftwareSerial Bluetooth(2, 3); // RX, TX (10 , 11 )
SoftwareSerial DFplayer(4, 5); // RX, TX (10 , 11 )

int SIZE = 1;
float V = 17; // швидкість
float f; // час руху вперед та назад
char *arr = new char[SIZE]; // масив для вхідних даних
int trigPin = 6;
int echoPin = 11;


#include <Stepper.h>
const int stepsPerRevolution = 360; 
Stepper myStepper(stepsPerRevolution, 7, 9, 8, 10);
void setup() { 
  //Serial.begin (9600);
  Bluetooth.begin(9600);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  
  
  myStepper.setSpeed(60);
  mp3_set_serial (DFplayer);
  delay(1);  
  mp3_set_volume (30);
}

void loop() {
enableMotors();
 Bluetooth.begin(9600);
if (Bluetooth.available()) {
  char c = Bluetooth.read();
  if(c== '1'|| c == '2'|| c== '3'||c== '4'){ // якщо прийшли дані
arr[SIZE-1] = c;
SIZE++;
}
else if (c == '0'){
for(int i = 0; i < SIZE-1;i++){
  enableMotors();
  switch(arr[i]){
  case '1':
   DFplayer.begin(9600);
  mp3_play (2);
 delay(3000);
 f = TimeOfDelay();
 pinMode(pinPWMB, OUTPUT);
   move(forward, 180); // вперед   
   delay(f);
    fullStop();
  break;
  case '2':
   DFplayer.begin(9600);
 mp3_play (3);
 delay(3000); 
 for(int i = 0; i < 3; i++){
  myStepper.step(stepsPerRevolution);
 delay(1000);
 }
   f = TimeOfDelay();
for(int i = 0; i < 3; i++){
  myStepper.step(-stepsPerRevolution);
 delay(1000);
 }
   pinMode(pinPWMB, OUTPUT);
   move(backward, 180); // назад 
  f = f - 1000;
   delay(f);
   fullStop();
  break;
  case '3':
   DFplayer.begin(9600);
  mp3_play (4);
  delay(3000);
   turn(clockwise, 180); // вправо
  delay(500);
    fullStop();
  break;
  case '4':
   DFplayer.begin(9600);
  mp3_play (6);
  delay(3000);
   turn(counterClockwise, 180); // вліво :
  delay(500);
    fullStop();
  break;
  default:
   DFplayer.begin(9600);
   mp3_play (5); // команди не знайдено/не розпізнано
   delay(4000);
  break;
   }
 }
 delete[] arr;
 SIZE = 1;
}
}
 }
void move(int direction, int speed)
{
   if (direction == forward)
   {
      moveMotorForward(pinMotorA, speed);
      moveMotorForward(pinMotorB, speed);
   }
   else
   {
      moveMotorBackward(pinMotorA, speed);
      moveMotorBackward(pinMotorB, speed);
   }
}
 
void turn(int direction, int speed)
{
   if (direction == forward)
   {
      moveMotorForward(pinMotorA, speed);
      moveMotorBackward(pinMotorB, speed);
   }
   else
   {
      moveMotorBackward(pinMotorA, speed);
      moveMotorForward(pinMotorB, speed);
   }
}
 
void fullStop()
{
   disableMotors();
   stopMotor(pinMotorA);
   stopMotor(pinMotorB);
}
 
void moveMotorForward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], speed);
}
 
void moveMotorBackward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
 
   analogWrite(pinMotor[0], speed);
}
 
void stopMotor(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], 0);
}
 
void enableMotors()
{
   digitalWrite(pinSTBY, HIGH);
}
 
void disableMotors()
{
   digitalWrite(pinSTBY, LOW);
}

float TimeOfDelay()// HC-SR04 
{
 disableMotors();
pinMode(echoPin, INPUT); 
int duration, cm; 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH); 
Serial.begin(9600);
cm = duration / 58;
Serial.println(cm);
float res = (cm / V)*650;
Serial.println(res);
enableMotors();
return res;
}
