#include <Servo.h>  

int irSensorPin = 7;      
unsigned long startTime = 0;
bool wafferPassed = false;

Servo myServo;           
int servoPin = 9;        

// LED Pins
int ledPins[] = {2, 3, 4, 5};  // LEDs connected to pins 2, 3, 4, 5

void setup() {
  pinMode(irSensorPin, INPUT);  
  Serial.begin(9600);           
  myServo.attach(servoPin);      
  myServo.write(0);  // Setting servo to 0 degrees
  
  // Seting LED pins as OUTPUT and turn them off initially
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  int sensorValue = digitalRead(irSensorPin);  // Reading the sensor value

  if (sensorValue == HIGH && !wafferPassed) {  
    // Wiper passing the sensor
    startTime = millis();
    wafferPassed = true;
    Serial.println("Wiper Passed, Timer Started");
  } 
  else if (sensorValue == LOW && wafferPassed) {  
    // Wiper returns
    unsigned long endTime = millis();
    unsigned long timeDifference = (endTime - startTime);

    Serial.print("Wiper Returned, Time Taken: ");
    Serial.print(timeDifference);
    Serial.println("ms");

    // Determining speedometer position
    int servoAngle = getServoAngle(timeDifference);
    
    // Moving speedometer
    myServo.write(servoAngle);
    Serial.print("Servo moved to: ");
    Serial.print(servoAngle);
    Serial.println(" degrees");

    // Controlling LEDs
    controlLEDs(servoAngle);

    // Reset for the next cycle
    wafferPassed = false;
  }
  
  delay(50);
}

int getServoAngle(unsigned long timeDiff) {
  if (timeDiff >= 1000) {
    return 0;  
  } else if (timeDiff >= 750 && timeDiff <= 1000) {
    return 45;
  } else if (timeDiff >= 500 && timeDiff <= 750 ) {
    return 90;
  } else if (timeDiff >= 250 && timeDiff <= 500 ) {
    return 135;
  } else {
    return 180;
  }
}

void controlLEDs(int angle) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  // Lighting up the corresponding LED
  if (angle == 45) {
    digitalWrite(ledPins[0], HIGH);
  } else if (angle == 90) {
    digitalWrite(ledPins[1], HIGH);
  } else if (angle == 135) {
    digitalWrite(ledPins[2], HIGH);
  } else if (angle >= 180) {
    digitalWrite(ledPins[3], HIGH);
  }
}
