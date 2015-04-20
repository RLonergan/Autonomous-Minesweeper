//Pid Constants

//What is the desired value?
//const int setPoint = 20;
//This is now a function input

//PID Error Constsants
//const float kP = 0.5 * setPoint;
//const float kI = 0;
//const float kD = 0.05 * setPoint;

//Pid Varibales
int proportional = 0;
int integral = 0;
int derivative = 0;
int lastProportional = 0;
int error = 0;

int pidCalculation(int input, int setPoint)
{
  const float kP = 40;
  const float kI = 0;
  const float kD = 4;

  /* Perform a Proportional Integral Derivative (PID) process control algorithm on the sensor input 
   * Returns error value meant to be applied to motor speeds
   * pidCalculation(float) -> int
   */
   
  proportional = input - setPoint;
  integral = integral + proportional;
  derivative = proportional - lastProportional;
  lastProportional = proportional;
  
  error = int(proportional * kP + integral * kI + derivative * kD);
  error = constrain(error, -maxSpeed, maxSpeed);
  return error;
}

