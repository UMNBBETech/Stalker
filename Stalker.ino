#include <HX711_ADC.h>
#include <Wire.h>
#include <SparkFun_MMA8452Q.h>
#include <SparkFun_RFD77402_Arduino_Library.h>
#include <gp20u7.h>

HX711_ADC LoadCell(4, 5);
MMA8452Q accel;
RFD77402 myDistance;
GP20U7 gps = GP20U7(Serial);

Geolocation currentLocation;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  LoadCell.begin();
  LoadCell.start(0);
  LoadCell.setCalFactor(696.0);
  accel.init();
  gps.begin();
  myDistance.begin();
}

void loop() {
  while(!gps.read()){
    digitalWrite(3, LOW);
    delay(100);
  }
  currentLocation = gps.getGeolocation();
  LoadCell.update();
  Serial.print(millis());
  Serial.print(",");
  Serial.print(currentLocation.latitude,5);
  Serial.print(",");
  Serial.print(currentLocation.longitude,5);
  Serial.print(LoadCell.getData());
  Serial.print(",");
  accel.read();
  Serial.print(accel.x, 3);
  Serial.print(",");
  Serial.print(accel.y, 3);
  Serial.print(",");
  Serial.print(accel.z, 3);
  Serial.print(",");
  Serial.print(((atan2(accel.x,accel.z))*180)/PI ,3);
  if(((atan2(accel.x,accel.z))*180)/PI>=45){
    digitalWrite(5, LOW);
  }
  Serial.print(",");
  myDistance.takeMeasurement();
  Serial.print(myDistance.getDistance());
  Serial.println();
  digitalWrite(3, HIGH);
}
