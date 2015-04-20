/*
 * TODO:
 * calcOrientation
 * PID
 * 
 */


int readDistance()
{
  /*
  Read the Ultrasonic sensor and return the distance in cm
  readDistance(None) -> int
  */
  float time = sonar.ping();
  float distance_cm = time / 57;
  
  //Serial.print(distance_cm);
  //Serial.print("\t");
  //Serial.println(distance2_cm);
  
  return distance_cm;
}

void initMagnet()
{
  //sourced from Sparkfun Buildr HMC5883 test code
  Wire.begin();
  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(Magnet_Address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  //end of sourced code
}

int readMagnet()
{
  /*
  Read the Magnetometer and return a single absolture value of the readings
  readMagnet(None) -> int
  */
  int x,y,z;
  int ABS = 0;
  
  //sourced from Sparkfun Buildr HMC5883 test code
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(Magnet_Address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();


  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(Magnet_Address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
    
    //Addition made
    //Calculates the absolute magnatude of all 3 axis readings
    ABS = sqrt(pow(x,2)+pow(y,2)+pow(z,2)); 
  }
  //End scourced code
  return ABS;
}


