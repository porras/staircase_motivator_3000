#include <Ultrasonic.h>

Ultrasonic ultrasonic1(7);
Ultrasonic ultrasonic2(8);
int led = 12;
int output = 10;
int stable_distance1 = 0;
int stable_distance2 = 0;
bool stable = true;
bool already_setup = false;

void setup() {
  delay(1000);
  Serial.println("Setting up");
  Serial.begin(9600);
  stable_distance1 = get_stable_distance(ultrasonic1);
  Serial.print("Stable distance1: ");
  Serial.println(stable_distance1);
  stable_distance2 = get_stable_distance(ultrasonic2);
  Serial.print("Stable distance2: ");
  Serial.println(stable_distance2);

  pinMode(led, OUTPUT);
  Serial.println("Setting up done");
  already_setup = true;
}


void loop() {
  if (already_setup) {
    sense_motion("GO!", stable_distance1, ultrasonic1);
    sense_motion("STOP!", stable_distance2, ultrasonic2);
  } else {
    delay(100);
  }
}

void sense_motion(char* command, int stable_distance, Ultrasonic ultrasonic){
  if (stable) {
      if (!at_stable_distance(stable_distance, ultrasonic)) {
        stable = false;
        blink(command);
        delay(2000);
      }
    } else {
      if (at_stable_distance(stable_distance, ultrasonic)) {
        stable = true;
      }
    }
}

bool at_stable_distance(int stable_distance, Ultrasonic ultrasonic) {
  int current_distance = get_distance(ultrasonic);
  int difference = stable_distance - current_distance;
  Serial.print(stable_distance);
  Serial.print(",");
  Serial.print(current_distance);
  Serial.print(",");
  Serial.println(difference);
  return difference < 3;
}

void blink(char* command) {
  digitalWrite(led, HIGH);
  Serial.println(command);
  delay(100);
  digitalWrite(led, LOW);
}

int get_distance(Ultrasonic ultrasonic) {
  delay(50);
  ultrasonic.MeasureInCentimeters();
  int distance = ultrasonic.RangeInCentimeters;
  return distance;
}

int get_stable_distance(Ultrasonic ultrasonic) {
  int previous_measure;
  int current_measure = get_distance(ultrasonic);
  
  do
  {
    previous_measure = current_measure;
    delay(50);
    current_measure = get_distance(ultrasonic);
  } while (current_measure != previous_measure);
  
  return current_measure;
}

