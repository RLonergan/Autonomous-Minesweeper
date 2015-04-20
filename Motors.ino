void driveMotor(int left, int right)
{
  /*
  Drive the motors, int range of -255 to +255, ie full reverse, full forward
  To stop the motors Drive(0, 0)
  Drive(int, int) -> None
  */
  
  //Limit the input variables to the range -255 to +255
  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);
  
  
  //Use Sign function to detirmine motor direction
  digitalWrite(LEFT_MOTOR_DIRECTION, Sign(left));
  digitalWrite(RIGHT_MOTOR_DIRECTION, Sign(right));
  
  //Set Motor Speed
  analogWrite(LEFT_MOTOR_SPEED, abs(left));
  analogWrite(RIGHT_MOTOR_SPEED, abs(right));
}


//Math Sign function
int Sign(int n)
{
  //Is n a postive number?
  if (n >= 0)
  {
    return 1;
  }
  else if (n < 0)
  {
    return 0;
  }
}

//Servo Constants
int pos = 0;

//Start position needs to around 90 rather than 0 az min position appears to be around 70
int pos_start = 70;
int pos_end = 180;
int time = 0;
int last_time = 0;
boolean sweep_left = true;

void servoSweep(int duration)
{
  /* Sweep the servo arm from start_pos to end_pos, the time taken to do so should match duration
  
  servoSweep(int) -> None
  */
  time = millis() % duration;
  
  if (time < last_time)
    sweep_left = !sweep_left;
  
  last_time = time;
  
  if (sweep_left == true)
  {
    pos = map(time, 0, duration, pos_start, pos_end);
  }
  else
  {
    pos = map(time, 0, duration, pos_end, pos_start);
  }
  pos = constrain(pos, pos_start, pos_end);
  servo.write(pos);
}

void servo_seek()
{
  int max = 0;
  int max_index = 0;
  
  
  

  servo.write(pos_start);
  delay(300);
  
  int temp[pos_end-pos_start];
  
  for (pos = pos_start; pos <= pos_end; pos++)
  {
    servo.write(pos);
    temp[pos-pos_start] = readMagnet();
    if (temp[pos-pos_start] > max)
    {
      max = temp[pos-pos_start];
      max_index = pos-pos_start;
    }
    delay(15);
  }
  servo.write(max_index + pos_start);
  Serial.print(max_index);
  Serial.print("   ");
  Serial.println(max);
  delay(1000);

  /*
  else
  {
    servo.write(pos_end);
    delay(300);
    
    int temp[pos_end-pos_start];
    
    for (pos = pos_end; pos >= pos_start; pos--)
    {
      servo.write(pos);
      temp[pos-pos_start] = readMagnet();
      if (temp[pos-pos_start] > max)
      {
        max = temp[pos-pos_start];
        max_index = pos-pos_start;
      }
      delay(15);
    }
    servo.write(max_index + pos_start);
    Serial.print(max_index);
    Serial.print("   ");
    Serial.println(max);
    delay(1000);
  }*/
  
    
}


