#include <Ultrasonic.h>

Ultrasonic ultrasonic(7);
int led = 12;
int output = 10;
int stable_distance = 0;
bool stable = true;
bool already_setup = false;

void setup() {
  delay(1000);
  Serial.println("Setting up");
  Serial.begin(9600);
  stable_distance = get_stable_distance();
  Serial.println(stable_distance);
  pinMode(led, OUTPUT);
  Serial.println("Setting up done");
  already_setup = true;
}

void loop() {
  if (already_setup) {
    if (stable) {
      if (!at_stable_distance()) {
        stable = false;
        blink();
        delay(2000);
      }
    } else {
      if (at_stable_distance()) {
        stable = true;
      }
    }
  } else {
    delay(100);
  }
}

bool at_stable_distance() {
  int current_distance = get_distance();
  int difference = stable_distance - current_distance;
  Serial.print(stable_distance);
  Serial.print(",");
  Serial.print(current_distance);
  Serial.print(",");
  Serial.println(difference);
  return difference < 3;
}

void blink() {
  digitalWrite(led, HIGH);
  Serial.println("GO!");
  delay(100);
  digitalWrite(led, LOW);
}

int get_distance() {
  delay(50);
  ultrasonic.MeasureInCentimeters();
  int distance = ultrasonic.RangeInCentimeters;
  return distance;
}

int get_stable_distance() {
  int previous_measure;
  int current_measure = get_distance();
  
  do
  {
    previous_measure = current_measure;
    delay(50);
    current_measure = get_distance();
  } while (current_measure != previous_measure);
  
  return current_measure;
}

