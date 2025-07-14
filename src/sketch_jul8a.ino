#include "AlfredoCRSF.h"
#include <Servo.h>

AlfredoCRSF crsf;

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

void setup() {
  // put your setup code here, to run once:
    Serial1.begin(420000);
    crsf.begin(Serial1);
    esc1.attach(3);
    esc2.attach(4);
    esc3.attach(5);
    esc4.attach(6);
}

void loop() {
  // put your main code here, to run repeatedly:
  crsf.update();
  int throttle = map(crsf.getChannel(3),1000,2000,0,1000);
  int yaw = map(crsf.getChannel(4), 1000, 2000,-500, 500);
  int pitch = map(crsf.getChannel(2), 1000, 2000, -500, 500);
  int roll = map(crsf.getChannel(1), 1000,2000, -500, 500);

  int m1 = constrain(throttle + pitch + roll - yaw, 1000, 2000);
  int m2 = constrain(throttle + pitch - roll + yaw, 1000, 2000);
  int m3 = constrain(throttle - pitch + roll + yaw, 1000, 2000);
  int m4 = constrain(throttle - pitch - roll - yaw, 1000, 2000);

  esc1.writeMicroseconds(m1);
  esc2.writeMicroseconds(m2);
  esc3.writeMicroseconds(m3);
  esc4.writeMicroseconds(m4);



}
