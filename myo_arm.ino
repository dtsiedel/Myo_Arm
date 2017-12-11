#include <Servo.h>

//to exit immediately
#define EXITONSTART 1

class Finger
{
public:
  void attach(int a_pin);
  void mapToFinger(int input);
  void setInitialReadings();
  void readFSR();
  void printSignals();
  Servo servo;
  String name;
  int pin;
  int pinIn;
  int lowerBound;
  int upperBound;
  int inMin;
  int inMax;
  int currentReading;
  int previousReading;
  int previousTime;
  float SA1;
  float RA;
  int cooldown; 
  static int length;
private:
  void moveFinger(int location);
};
float derivative_approximation(int dt, int difference);


int myoPin = A0; 
int myoMin = 90; //80
int myoMax = 300; 
int cooldown = 0;
int SA1_total = 0;
Finger fingers[5]; 

int Finger::length = 0; //initialize static, I think it defaults to zero but this is to be safe

//attach to servo, set name for display purposes
void Finger::attach(int a_pin)
{
  pin = a_pin;
  switch(pin) //FINGER PINS (which arduino out to plug into)
  {
    //case 2: this->name = "PALM"; this->lowerBound = 60; this->upperBound = 130; break; //not sure if I want palm to be included or not    
    
    case 3: this->name = "THUMB"; this->pinIn = A1; this->lowerBound = 135; this->upperBound = 180; break;
    case 4: this->name = "INDEX";  this->pinIn = A2; this->lowerBound = 115; this->upperBound = 46;break;
    case 5: this->name = "MIDDLE"; this->pinIn = A3; this->lowerBound = 65; this->upperBound = 150; break;
    case 6: this->name = "RING";  this->pinIn = A4; this->lowerBound = 95; this->upperBound = 175; break;
    case 7: this->name = "PINKY";  this->pinIn = A5; this->lowerBound = 95; this->upperBound = 155; break;
    default: this->name = "ERROR_BAD_FINGER"; lowerBound = 90; this->upperBound = 90;
  }

  this->servo.attach(a_pin);
  Serial.print("Attached "); Serial.print(this->name); Serial.println();
  
  Finger::length++;
}

//convert value from input range to the
//finger's output range, then write that value
//to the finger
void Finger::mapToFinger(int input)
{
  int output = map(input, myoMin, myoMax, this->lowerBound, this->upperBound);
  //int output = map(0.5, 0, 1, this->lowerBound, this->upperBound); //turn off fingers for testing
  
  this->moveFinger(output);
}

//actually do the write to the servo pin
//only call this from mapToFinger(),
//because it maps our bounds appropriately
void Finger::moveFinger(int location)
{
  this->servo.write(location);
}

//get force applied to the sensor at any given moment
void Finger::readFSR()
{
  int current_time = millis();
  int dt = current_time - this->previousTime;
  this->previousTime = current_time;
  this->previousReading = this->currentReading;
  this->currentReading = constrain(analogRead(this->pinIn), this->inMin, this->inMax);
  this->SA1 = this->currentReading;
  this->RA = derivative_approximation(dt, this->currentReading - this->previousReading);
}

//handler for when the derivative of SA1 exceeds threshold
void handleRASpike(float RA, String source)
{
    Serial.print("RA Spike: "); Serial.print(RA); Serial.print(" "); Serial.println(source);
    analogWrite(10, 255);
    cooldown = 20;
}

//write either max or min to the RA spike motor
void tickRA()
{
  if(cooldown <= 0)
  {
    analogWrite(10, 0);
  }
  else
  {
    analogWrite(10, 255);
    cooldown--;
  }
}

//constantly called to write current pressure to SA1 motor
void tickSA1(int value, int pin)
{
   int output = constrain(value, 0, 5*750);
   output = map(output, 0, 5*750, 0, 255);
   //Serial.print(pin); Serial.print(" "); Serial.println(output);
   analogWrite(pin, output);
}

//print out the current SA1 and RA
void Finger::printSignals()
{
  int threshold = 10;
  if(abs(this->RA) > threshold)
  {
    handleRASpike(this->RA, this->name);
  }
  else
  {
    tickRA();
  }
}

//approximate derivative with (f(t+dt) - f(t)) / dt
float derivative_approximation(int dt, int difference)
{
  return float(difference) / float(dt);
}

//call this in setup to zero the readings for each finger
void Finger::setInitialReadings()
{
  this->inMin = 70;
  this->inMax = 750;
  this->currentReading = 0;
  this->previousReading = 0;
  this->previousTime = 0;
}

//reads from the myoPin, and does any
//work that is needed to correct that
//which for now is nothing
//later might need some smoothing or something
int getMuscleActivity()
{
  int in = analogRead(myoPin);
  in = constrain(in, myoMin, myoMax);
  return in;
}

//TODO
//dynamically find bounds of myo input
//on startup by playing a tone to indicate 
//the user should rest or flex their arm
//should be more accurate and customizable
//than trying to hardcode it
void getInputBounds()
{
  
}

void setup() 
{
  if(EXITONSTART) exit(0); //quit before doing anything if EXIT flag is set
  Serial.begin(9600);

  for(int i = 0; i < 5; i++)
  {
    fingers[i].setInitialReadings();
    fingers[i].attach(i+3);
  }

  getInputBounds();
  cooldown = 0;
}

void loop() 
{
  int myoIn = getMuscleActivity();
  int SA1_total = 0;
  for(int i = 0; i < Finger::length; i++)
  {
    fingers[i].readFSR();
    fingers[i].printSignals();
    fingers[i].mapToFinger(myoIn);
    SA1_total += (fingers[i].SA1);
  }

  //tickSA1(SA1_total, 9);
  tickSA1(SA1_total, 11);

  delay(1);
  
}

