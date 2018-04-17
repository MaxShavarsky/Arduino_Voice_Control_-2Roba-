//----------------------------------------------------------//
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
#include <AccelStepper.h>                                   // кроковий двигун
AccelStepper stepper(4, 7,8,9,10);                          //
//----------------------------------------------------------//
int COUNT = 0;                                              //
int DELAYCOUNT = 0;                                         //
int TEMPTIME = 500;                                         //
int SIZE = 1;                                               //
const int SPEED = 17;                                       //
float TIME =0;                                              //
unsigned int LIMIT1 = 0;                                    // змінні
unsigned int LIMIT2 = 0;                                    //
int LEFT = 0,RIGHT =0;                                      //
int WAY = 0;                                                //
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
  Bluetooth.begin(9600);
//----------------------------------------------------------//
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
//----------------------------------------------------------//
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(50.0);
  mp3_set_serial (DFplayer); 
  delay(1); 
  mp3_set_volume (30);
//----------------------------------------------------------//
}
void loop()
{
if (Bluetooth.available())
  {
    char c = Bluetooth.read();
  if(c == '1' || c == '2'||c == '3'||c == '4'||c == '5'||c == '6'||c == '7'||c == '8'||c == '9')
    {
    comand = comand + c;
    SIZE++;
    }
    else if (c == '0')  
    {
   Serial.begin(9600);
    //for(int i = 0; i < SIZE;i++){
      Serial.print("comand " + comand);
    //}
    delay(1000);
  for(int i = 0; i < SIZE;i++)
  {
    if(comand[i] == '1'||comand[i] == '2')
    {
      temp = comand[i+1];
      Serial.print("temp " + temp);
      way[i] = temp.toInt();
      Serial.print("way[i] ");
      Serial.print(way[i]);
      way[i] = way[i] * 100;
      Serial.print("way[i] ");
      Serial.print(way[i]);
      comand[i+1] = "0";
      Serial.print("comand " + comand);
     delay(1000);
    }
  }
for(int i = 0; i < SIZE;i++)
{
  enableMotors();
  switch(comand[i]){
    case '1':
    DFplayer.begin(9600);
       mp3_play (2);
       delay(3000);
       ALGORITHM(i);
    break;
    case '2':
       turn(counterClockwise, 180); // вліво
       delay(1000);
       fullStop();
       ALGORITHM(i);
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
      turn(counterClockwise, 180); // вліво
      delay(500);
      fullStop();
    break;
    default:
    break;
  }
}
COUNT = 0;
TEMPWAY = 500;
delete[] way;
SIZE = 1;
DELAYCOUNT = 0;
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
Serial.begin(9600);
duration = pulseIn(echoPin, HIGH);
Serial.println("Duration ");
Serial.println(duration); 
cm = duration / 58;
Serial.println("CM ");
Serial.println(cm);
enableMotors();
return cm;
}
//----------------------------------------------------------//
void ALGORITHM(int index)
{
       Serial.begin(9600);
       WAY = CM();
        Serial.print("WAY ");
      Serial.print(WAY);
       delay(1000);
    if(WAY > way[index])
    {
       TIME = (WAY / SPEED) * 650;
       if(TIME < 0){
        return;
       }
       pinMode(pinPWMB, OUTPUT);
       move(forward, 180); // вперед :  
       delay(TIME);
       fullStop();
    }
    else
    {
       way[index] = way[index] - WAY;
       Serial.println("way[index] ");
        Serial.println(way[index]);
       TIME = ((WAY / SPEED)* 650)-150;
       if(TIME < 0){
        return;
       }
       Serial.println("TIME ");
        Serial.println(TIME);
       pinMode(pinPWMB, OUTPUT);
       move(forward, 180);  
       delay(TIME);
       fullStop();
       delay(2000);
     
       if(CHECK())
       {
          enableMotors();
          turn(counterClockwise, 180); // вліво
          delay(400);
          fullStop();
        stepper.moveTo(500);
        while(true){
          if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
        }
         delay(2000);
       }
       else
       {
          enableMotors();
          turn(clockwise, 180); // вправо 
          delay(400);
          fullStop();
        stepper.moveTo(-500);
         while(true){
          if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
        }
         delay(2000);
       }
       if(!remember){
      for(int j = 0; j < 3;j++)
      {
        enableMotors();
        if(j == 1)
        {
          LIMIT1 = millis();
          while(true)
          {    
            TEMPWAY = CM();
            Serial.println("TEMPWAY = ");
             Serial.print(TEMPWAY);
            delay(2000);
            DELAYCOUNT +=1;
            if(TEMPWAY != 0){
            move(forward, 180); // вперед 
            delay(500);
            }
            if(TEMPWAY > 15)
            {
              fullStop();
              LIMIT2 = millis() - LIMIT1-2000;
              Serial.println("LIMIT2 ");
              Serial.println(LIMIT2);
              way[index] = way[index] - (SPEED * ((LIMIT2/1000)/DELAYCOUNT));
              Serial.println("way[index] = ");
        Serial.print(way[index]);
         delay(2000);
              enableMotors();
              turn(counterClockwise, 180); // вліво
              delay(400);
              fullStop();
              break;
            }
          }
        }
        else if(j == 0)
        {
          while(true)
          {
            TEMPWAY = CM();
            Serial.println("TEMPWAY = ");
              Serial.print(TEMPWAY);
            delay(2000);
           if(TEMPWAY != 0){
            move(forward, 180); // вперед 
            delay(500);
            COUNT = COUNT + 1;
            }
            if(TEMPWAY > 15)
            {
              fullStop();
              enableMotors();
              turn(counterClockwise, 180); // вліво
              delay(400);
              fullStop();
              break;
            }
          }
        }else
        {
          Serial.println("COUNT = ");
          Serial.print(COUNT);
            move(forward, 180); // вперед 
            TEMPTIME = TEMPTIME * COUNT;
            delay(TEMPTIME);
            turn(clockwise, 180); // вправо 
            delay(400);
            fullStop();  
            break;
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
          LIMIT1 = millis();
          
          while(true)
          {
            TEMPWAY = CM();
            Serial.println("TEMPWAY = ");
              Serial.print(TEMPWAY);
            delay(2000);
            DELAYCOUNT+=1;
            Serial.println("DELAYCOUNT = ");
            Serial.print(DELAYCOUNT);
            if(TEMPWAY != 0){
            move(forward, 180); // вперед 
            delay(500);
            }
            
            if(TEMPWAY > 15)
            {
              fullStop();
              LIMIT2 = millis() - LIMIT1-2000;
              Serial.println("LIMIT2 = ");
              Serial.print(LIMIT2);
              way[index] = way[index] - (SPEED * ((LIMIT2/1000)/DELAYCOUNT));
              Serial.println("way[index] = ");
        Serial.print(way[index]);
         delay(2000);
              enableMotors();
              turn(clockwise, 180); // вправо 
              delay(400);
              fullStop();
              break;
            }
          }
        }
        else if (j == 0)
        {
          while(true)
          {
            TEMPWAY = CM();
            Serial.println("TEMPWAY = ");
              Serial.print(TEMPWAY);
            delay(2000); 
           if(TEMPWAY != 0){
            move(forward, 180); // вперед 
            delay(500);
            COUNT = COUNT +1;
            }
            if(TEMPWAY > 15)
            { 
              fullStop();
              enableMotors();
              turn(clockwise, 180); // вправо 
              delay(400);
              fullStop();
              break;
            }
          }
        }
        else
        {
          Serial.println("COUNT = ");
          Serial.print(COUNT);
          move(forward, 180); // вперед 
            TEMPTIME = TEMPTIME * COUNT;
            delay(TEMPTIME);
            turn(counterClockwise, 180); // вліво
            delay(400);
            fullStop();  
            break;
        }
      
      }
       }
       stepper.moveTo(0);
         while(true){
          if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
        }
         delay(2000);
         enableMotors();
         TIME = (way[index] / SPEED)*1000;
         Serial.println("TIME LAST = ");
         Serial.print(TIME); 
         move(forward, 180); // вперед   
         delay(TIME);
         fullStop();
}
}
//----------------------------------------------------------//
bool CHECK()
{
  stepper.moveTo(-550);
         while(true){
          if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
  }
       LEFT = CM();
       delay(2000);
  stepper.moveTo(500);
  while(true){
    if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
  }
       RIGHT = CM();
        delay(2000);
        stepper.moveTo(0);
        while(true){
          if(stepper.distanceToGo()==0){
            break;
          }
         stepper.run();
  }
         delay(2000);
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
