A robotic prosthetic made using Arduino and the same robot arm from my previous robot arm build.

Uses myoelectric sensors to detect transdermal electric signals that are indicative of muscle activity (as opposed to the previous version, which took input from flex sensors in a glove). This arm/technology could be used by limb-different individuals to control the movement of the arm by flexing the muscle of the forearm, or shoulder depending on the development of the residual arm.

Additionally, uses force sensitive resistors to detect when the hand is touching something, and transmits the signal to haptic feedback motors in a cuff on the user's upper arm. The system encodes both direct force applied and the beginning and end of touch (by derivative approximation of force). 

Naturally a production version would use stronger and lighter materials than the balsa wood of this arm (likely 3D-printed materials like ABS or carbon fiber), for reliability and comfort of the user.

<a align='center' href="https://www.youtube.com/watch?v=VBaf7skSzEs"><img src="https://github.com/dtsiedel/Myo_Arm/blob/master/arm.gif" height="300" width="200"></a>
