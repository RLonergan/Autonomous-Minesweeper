/* Team Red Rover
 * P01 Friday 12:00-2:00
 * 
 *
 */

//Libraries:
#include <Wire.h> //I2C library
#include <NewPing.h> //Ultrasonic sensor library
#include <Servo.h> //Servo motor library

//Constants:
//Motor Driver pins
#define LEFT_MOTOR_DIRECTION 4
#define LEFT_MOTOR_SPEED 5
#define RIGHT_MOTOR_DIRECTION 7
#define RIGHT_MOTOR_SPEED 6

//I2C Driv addresses
#define Magnet_Address 0x1E 

//Ultrasonic Sensors
#define ULTRA_TRIGGER_PIN 10
#define ULTRA_ECHO_PIN 11
#define MAX_DISTANCE 300 //Maximum distance expected from ultrasonic sensor

//Gyro Sensor Addresses
#define Gyro_Address 0x69
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

//Misc Output Pins
#define LED_PIN 13
#define BUZZER_PIN 2
#define PUMP_BUTTON 8
#define PUMP 3

//define ultrasonic sensor as NewPing object
NewPing sonar(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, MAX_DISTANCE);

//Define servo motor objects
Servo servo;


//Global Variables:
const int Magnet_Threshold = 1800;

//Servo constants
const int sweep_duration = 825;

//Are the sensors being debugged via Serial?
const int Debug = true;

const int maxSpeed = 140;
//120 works


void setup()
{
  //Initialise output pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP, OUTPUT);
  
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
  
  //Initialise Sensors
  initMagnet();
  
  //Initialise Servo
  servo.attach(9);
  
  //Are the sensors being debugged?
  if (Debug == true)
  {
    Serial.begin(9600);
  }
}

int leftSpeed, rightSpeed;

long global_time = 0;
long wait_time = 0;
float radius = 0;

long last_detection_time = 0;

void loop()
{
  global_time = millis() - wait_time;
  
  radius = 0.0002 * global_time + 27.265;
  int globalSpeed = maxSpeed - (radius - 27.265) * 0.6;
  
  int distance = readDistance();
  int error = pidCalculation(distance, radius);
  int magnet = readMagnet();
  
  if (error <= 0)
  {
    leftSpeed = globalSpeed;
    rightSpeed = globalSpeed + error;
  }
  else if (error > 0)
  {
    leftSpeed = globalSpeed - error;
    rightSpeed = globalSpeed;
  }
  else
  {
    driveMotor(0, 0);
  }
  
  //Check for Manual Pump Activation
  if (digitalRead(PUMP_BUTTON) == HIGH)
  {
    analogWrite(PUMP, 50);
  }
  else
  {
    analogWrite(PUMP, 0);
  }
  
  driveMotor(leftSpeed, rightSpeed);
  
  
  servoSweep(sweep_duration);
  
  
  if (magnet > Magnet_Threshold)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    driveMotor(0, 0);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    servo_seek();
    analogWrite(PUMP, 50);
    delay(350);
    analogWrite(PUMP, 0);
    driveMotor(globalSpeed, globalSpeed * 0.75);
    delay(800);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }
  /*
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("\t Error: ");
  Serial.print(error);
  Serial.print("\t LeftSpeed: ");
  Serial.print(leftSpeed);
  Serial.print("\t RightSpeed: ");
  Serial.print(rightSpeed);
  
  Serial.print("Radius: ");
  Serial.print(radius);
  Serial.print("\t Magnetometer: ");
  Serial.println(magnet);
  */
}
