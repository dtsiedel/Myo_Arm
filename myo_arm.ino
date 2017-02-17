# include <Servo.h>

//actually doing it right with OOP this time lol
class Finger
{
public:
  void attach(int a_pin);
  void mapToFinger(int input);
  Servo servo;
  String name;
  int pin;
  int lowerBound;
  int upperBound;
  static int length;
private:
  void moveFinger(int location);
};

int myoPin = A0; //placeholder
int myoMin = 0; //placeholder
int myoMax = 1023; //placeholder
Finger fingers[5]; 
int Finger::length = 0; //initialize static, I think it defaults to zero but this is to be safe
#define EXITONSTART 0 //flag for exiting before doing anything, for debugging


//attach to servo, set name for display purposes
void Finger::attach(int a_pin)
{
  pin = a_pin;
  switch(pin) //FINGER PINS (which arduino out to plug into)
  {
    case 2: name = "PALM"; lowerBound = 60; upperBound = 130; break; //not sure if I want palm to be included or not
    case 3: name = "THUMB"; lowerBound = 135; upperBound = 180; break;
    case 4: name = "INDEX";  lowerBound = 46; upperBound = 115;break;
    case 5: name = "MIDDLE"; lowerBound = 65; upperBound = 150; break;
    case 6: name = "RING"; lowerBound = 95; upperBound = 175; break;
    case 7: name = "PINKY"; lowerBound = 95; upperBound = 155; break;
    default: name = "ERROR_BAD_FINGER"; lowerBound = 90; upperBound = 90;
  }
  Finger::length++;
}

//convert value from input range to the
//finger's output range, then write that value
//to the finger
void Finger::mapToFinger(int input)
{
  int output = map(input, myoMin, myoMax, this->lowerBound, this->upperBound);
  this->moveFinger(output);
}

//actually do the write to the servo pin
//only call this from mapToFinger(),
//because it maps our bounds appropriately
void Finger::moveFinger(int location)
{
  this->servo.write(location);
}

//reads from the myoPin, and does any
//work that is needed to correct that
//which for now is nothing
//later might need some smoothing or something
int getMuscleActivity()
{
  int in = analogRead(myoPin);
  in = constrain(in, myoMin, myoMax);
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

  for(int i = 0; i <= 5; i++)
  {
    fingers[i].attach(i+2);
  }

  getInputBounds();

}

void loop() 
{
  int myoIn = getMuscleActivity();
  for(int i = 0; i <= Finger::length; i++)
  {
    fingers[i].mapToFinger(myoIn);
  }
}

