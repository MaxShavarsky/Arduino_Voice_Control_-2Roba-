//----------------------------------------------------------//
const int pinPWMA = 6; // живлення для моторчика №1        //
const int pinAIN2 = 7;                                      //
const int pinAIN1 = 8;                                      //
const int pinBIN1 = 9;                                      //
const int pinBIN2 = 10;                                     //
const int pinPWMB = 11;  // живлення для моторчика №2      //
const int pinSTBY = 12; // пін дозволу руху                 //
                                                            //
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
//----------------------------------------------------------//
#include <SoftwareSerial.h>                                 //
#include <DFPlayer_Mini_Mp3.h>                              // мп3 плеєр
SoftwareSerial Bluetooth(2, 3); // RX, TX (10 , 11 )        //
SoftwareSerial DFplayer(4, 5); // RX, TX (10 , 11 )         //
//----------------------------------------------------------//
int trigPin = 6;                                            //
int echoPin = 11;                                           //
float f;                                                    //
//----------------------------------------------------------// 
#include <Servo.h>                                          //кроковий двигун
Servo myservo;                                              //
//----------------------------------------------------------//
int COUNT = 0;                                              //
int DELAYCOUNT = 0;                                         //
int TEMPTIME = 300;                                         //
int SIZE = 1;                                               //
const int SPEED = 20;                                       //
float TIME = 0;                                              //
unsigned int LIMIT1 = 0;                                    // змінні
unsigned int LIMIT2 = 0;                                    //
int LEFT = 0,RIGHT =0;                                      //
float WAY = 0;                                              //
int TEMPWAY = 0;                                            //
int duration, cm;                                           //
String comand = "";                                         //
String temp = "";                                           //
int *way = new int[SIZE];                                   //
bool remember = true;                                       //
//----------------------------------------------------------//
void setup()
{
//----------------------------------------------------------//
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
//----------------------------------------------------------//
  //stepper.setMaxSpeed(300);
  //stepper.setAcceleration(50.0);
  myservo.attach(13);
  mp3_set_serial (DFplayer); 
  delay(1); 
  mp3_set_volume (30);
  
//----------------------------------------------------------//
}
void loop()
{
  myservo.write(90);
  delay(100);

  
  Bluetooth.begin(9600);
if (Bluetooth.available())
  {
    char c = Bluetooth.read();
  if(c == '1' || c == '2'||c == '3'||c == '4'||c == '5'||c == '6'||c == '7'||c == '8'||c == '9')
    {
    comand = comand + c;
    Serial.begin(9600);
    Serial.println(comand);
    SIZE++;
    }
    else if (c == '0')  
    {
  for(int i = 0; i < SIZE;i++)
  {
    if(comand[i] == '1'||comand[i] == '2')
    {
      temp = comand[i+1];
      way[i] = temp.toInt();
      way[i] = way[i] * 100;
      comand[i+1] = "0";
    }
  }
for(int i = 0; i < SIZE;i++)
{
  enableMotors();
  switch(comand[i]){
    case '1':
       ALGORITHM(i);
    break;
    case '2':
       turn(counterClockwise, 180); // вліво
       delay(800);
       fullStop();
       enableMotors();
       ALGORITHM(i);
    break;
    case '3':
      DFplayer.begin(9600);
      mp3_play (6);
      delay(3000);
      turn(clockwise, 180); // вправо
      delay(400);
      fullStop();
    break;
    case '4':
      DFplayer.begin(9600);
      mp3_play (4);
      delay(3000);
      turn(counterClockwise, 180); // вліво
      delay(400);
      fullStop();
    break;
    default:
    break;
  }
}
COUNT = 0;
TEMPWAY = 300;
delete[] way;
SIZE = 1;
DELAYCOUNT = 0;
comand = "";
}
}


}
//----------------------------------------------------------//

//----------------------------------------------------------//
int CM()// HC-SR04 
{
 disableMotors();
pinMode(echoPin, INPUT); 
int duration, cm; 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
Serial.println("duration");
Serial.println(duration);
cm = duration / 58;
Serial.println("cm");
Serial.println(cm);
enableMotors();
return cm;
}
//----------------------------------------------------------//
void ALGORITHM(int index)
{
  Serial.begin(9600);
       WAY = CM();
       Serial.println("WAY");
       Serial.println(WAY);
    if(WAY > way[index])
    {
      Serial.println("You here");
       TIME = ((WAY / SPEED) * 500)-600;
       Serial.println("TIME");
       Serial.println(TIME);
       if(TIME < 0){
        Serial.println("Oops");
        fullStop();
        return;
       }
       DFplayer.begin(9600);
       mp3_play (2);
       delay(3000);
       pinMode(pinPWMB, OUTPUT);
       move(forward, 180); // вперед :  
       delay(TIME);
       fullStop();
       return;
    }
    else
    {
      Serial.println("No,you here");
       way[index] = way[index] - WAY;
       Serial.println("way[index]");
       Serial.println(way[index]);
       TIME = ((WAY / SPEED)* 500)-600;
       Serial.println("TIME");
       Serial.println(TIME);
       if(TIME < 0){
        Serial.println("Oops oops");
        fullStop();
        return;
       }
       DFplayer.begin(9600);
       mp3_play (2);
       delay(3000);
       pinMode(pinPWMB, OUTPUT);
       move(forward, 180);  
       delay(TIME);
       fullStop();
       
    }
       if(CHECK())
       {
        Serial.println("Left");
          enableMotors();
          turn(clockwise, 180); // вправо 
          delay(400);
          fullStop();
          myservo.write(0);
          delay(1000);
       }
       else
       {
         Serial.println("Right");
          enableMotors();
          turn(counterClockwise, 180); // вліво
          delay(400);
          fullStop();
          myservo.write(180);
          delay(500);
       }
       
       if(!remember)
       {

      for(int j = 0; j < 3;j++)
      {
        enableMotors();
        if(j == 1)
        {
          Serial.println("Here 1");
          LIMIT1 = millis();
          while(true)
          {    
            TEMPWAY = CM();
            if(TEMPWAY != 0 && TEMPWAY <= 20){
            move(forward, 180); // вперед
            delay(200);
            }
            if(TEMPWAY > 20)
            {
              fullStop();
              LIMIT2 = millis() - LIMIT1-1000;
              way[index] = way[index] - (SPEED * ((LIMIT2/1000)));
              enableMotors();
              move(forward, 180); // вперед 
              delay(200);
              fullStop();
              delay(500);
              enableMotors();
              turn(clockwise, 180); // право
              delay(350);
              fullStop();
              delay(500);
              enableMotors();
              move(forward, 180); // вперед 
             delay(200);
              fullStop();
              break;
            }
          }
        }
        else if(j == 0)
        {
          Serial.println("Here 0");
          while(true)
          {
            TEMPWAY = CM();
           if(TEMPWAY != 0&& TEMPWAY <= 20){
            move(forward, 180); // вперед 
            delay(200);
            COUNT = COUNT + 1;
            }
            if(TEMPWAY > 20)
            {
              fullStop();
              delay(500);
              enableMotors();
              move(forward, 180); // вперед 
              delay(200);
              fullStop();
              delay(500);
              enableMotors();
              turn(clockwise, 180); // право
              delay(400);
               fullStop();
               delay(500);
              enableMotors();
              move(forward, 180); // вперед 
            delay(200);
              fullStop();
              break;
            }
          }
        }else if(j == 2)
        {
          Serial.println("Here 2");
            move(forward, 180); // вперед 
            Serial.println(COUNT);
            TEMPTIME = TEMPTIME * COUNT;
            if(TEMPTIME <= 0){
              Serial.println("Oops 2");
              fullStop();
              return;
            }else{
              delay(TEMPTIME);
            }
            fullStop();
            delay(500);
            enableMotors();
            turn(counterClockwise, 180); // вправо 
            delay(400);
             fullStop();
             delay(500);
              enableMotors();
            move(forward, 180); // вперед 
            delay(200);
            fullStop();  
        }
      }
       }
       else
       {
        for(int j = 0; j < 3;j++)
      {
        enableMotors();
        if(j == 1)
        {
          Serial.println("Here 1(1)");
          LIMIT1 = millis();
          
          while(true)
          {
            TEMPWAY = CM();
            if(TEMPWAY != 0&& TEMPWAY <= 20){
            move(forward, 180); // вперед 
            delay(200);
            }
            
            if(TEMPWAY > 20)
            {
              fullStop();
              LIMIT2 = millis() - LIMIT1-1000;
              way[index] = way[index] - (SPEED * ((LIMIT2/1000)));
              enableMotors();
              move(forward, 180); // вперед 
              delay(200);
              fullStop();
              delay(500);
              enableMotors();
              turn(counterClockwise, 180); // вправо 
              delay(400);
               fullStop();
               delay(500);
              enableMotors();
              move(forward, 180); // вперед 
            delay(200);
              fullStop();
              break;
            }
          }
        }
        else if(j == 0)
        {
          Serial.println("Here 0(0)");
          while(true)
          {
            TEMPWAY = CM();
           if(TEMPWAY != 0 && TEMPWAY <= 20){
            move(forward, 180); // вперед 
            delay(200);
            COUNT = COUNT +1;
            }
            if(TEMPWAY > 20)
            { 
              fullStop();
              delay(500);
              enableMotors();
              move(forward, 180); // вперед 
              delay(200);
              fullStop();
              delay(500);
              enableMotors();
              turn(counterClockwise, 180); // вправо 
              delay(400);
               fullStop();
               delay(500);
              enableMotors();
              move(forward, 180); // вперед 
            delay(200);
              fullStop();
              break;
            }
          }
        }
        else if(j == 2)
        {
          Serial.println("Here 2(2)");
          move(forward, 180); // вперед 
          Serial.println(COUNT);
            TEMPTIME = TEMPTIME * COUNT;
             if(TEMPTIME <= 0){
              Serial.println("Oops 2(2)");
              fullStop();
              return;
            }else{
              delay(TEMPTIME);
            }
            fullStop();
            delay(500);
              enableMotors();
            turn(clockwise, 180); // вліво
            delay(400);
             fullStop();
             delay(500);
              enableMotors();
            move(forward, 180); // вперед 
            delay(200);
            fullStop();
        }
      }
       }
       Serial.println("Not end");
         myservo.write(90);
         delay(500);
         enableMotors();
         if(way[index]>25)
         {
           TIME = ((way[index] / SPEED)*500)-600;
           move(forward, 180); // вперед 
          delay(TIME);
          fullStop();
         }else
         {
          move(forward, 180); // вперед 
          delay(1000);
          fullStop();
         } 
         Serial.println("The end");
}
//----------------------------------------------------------//
bool CHECK()
{
       myservo.write(180);  
       delay(1000);
       LEFT = CM();
           Serial.println(LEFT);
       myservo.write(0);  
       delay(1000);
       RIGHT = CM();

    Serial.println(RIGHT);
       myservo.write(90);  
       delay(1000);
         
       if(LEFT > RIGHT)
       {
        remember = true;
        return remember;
       }
       else
       {
        remember = false;
        return remember;
       }
}
//----------------------------------------------------------//
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
//----------------------------------------------------------// 
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
//----------------------------------------------------------// 
void fullStop()
{
   disableMotors();
   stopMotor(pinMotorA);
   stopMotor(pinMotorB);
}
//----------------------------------------------------------// 
//Funciones que controlan los motores
void moveMotorForward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], speed);
}
//----------------------------------------------------------// 
void moveMotorBackward(const int pinMotor[3], int speed)
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
 
   analogWrite(pinMotor[0], speed);
}
//----------------------------------------------------------// 
void stopMotor(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], 0);
}
//----------------------------------------------------------// 
void enableMotors()
{
   digitalWrite(pinSTBY, HIGH);
}
//----------------------------------------------------------// 
void disableMotors()
{
   digitalWrite(pinSTBY, LOW);
}
//----------------------------------------------------------//
