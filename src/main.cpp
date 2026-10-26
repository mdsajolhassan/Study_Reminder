#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1    

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
const int buzzer = 8;
const int studyLED = 9;   
const int breakLED = 10;  
const int startBtn = A0;
const int resetBtn = A1;

// Ultrasonic Sensor Pins
const int trigPin = 6;
const int echoPin = 7;

// Timer Variables
const int studyTime = 25; 
const int breakTime = 10; 
int remainingTime = studyTime;
bool running = false;
bool studyMode = true;
unsigned long previousMillis = 0;

const int maxDistance = 70; 

// Function Prototypes
void displayTime();
void alarm();
void resetTimer();
int getDistance();

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(studyLED, OUTPUT);
  pinMode(breakLED, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);

  digitalWrite(studyLED, LOW);
  digitalWrite(breakLED, LOW);

  if(!display.begin(i2c_Address, true)) {
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.println("Study Reminder");
  display.setCursor(0, 30);
  display.println("Press START");
  display.display();
}

void loop() {
  // START Button
  if (digitalRead(startBtn) == LOW && !running) {
    delay(200); 
    running = true;
    digitalWrite(studyLED, HIGH); // Study LED ON
    previousMillis = millis();    // ফিক্স: বাটন চাপার মুহূর্ত থেকে টাইম কাউন্ট হবে
    displayTime();
  }

  // RESET Button
  if (digitalRead(resetBtn) == LOW) {
    delay(200); 
    resetTimer();
  }

  if (running) {
    if (millis() - previousMillis >= 1000) {
      previousMillis = millis();

      if (studyMode) {
        int distance = getDistance();
        
        if (distance > 0 && distance <= maxDistance) {
          digitalWrite(studyLED, HIGH); 
          digitalWrite(breakLED, LOW);  
          remainingTime--;
          displayTime();
        } 
        else {
          digitalWrite(studyLED, LOW);  
          digitalWrite(breakLED, HIGH); 
          
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(0, 10);
          display.println("Student Absent!");
          display.setCursor(0, 30);
          display.println("Timer Paused...");
          display.display();
        }
      } 
      else {
        remainingTime--;
        displayTime();
      }

      if (remainingTime <= 0) {
        if (studyMode) {
          studyMode = false;
          remainingTime = breakTime;
          digitalWrite(studyLED, LOW);
          digitalWrite(breakLED, HIGH);

          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(0, 10);
          display.println("BREAK!");
          display.setTextSize(1);
          display.setCursor(0, 40);
          display.println("Relax & Rest");
          display.display();
          alarm();
        } else {
          studyMode = true;
          remainingTime = studyTime;
          digitalWrite(studyLED, HIGH);
          digitalWrite(breakLED, LOW);

          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(0, 10);
          display.println("STUDY!");
          display.setTextSize(1);
          display.setCursor(0, 40);
          display.println("Back to Work");
          display.display();
          alarm();
        }
        delay(2000);
        if(studyMode) displayTime();
      }
    }
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  if (duration == 0) return 999; 
  
  int distance = duration * 0.034 / 2;
  return distance;
}

void displayTime() {
  int minutes = remainingTime / 60;
  int seconds = remainingTime % 60;

  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (studyMode) {
    display.println("STUDY TIME");
  } else {
    display.println("BREAK TIME");
  }

  display.setTextSize(2); // সাইজ কমানো হয়েছে পাওয়ার ড্রপ রোধ করতে
  display.setCursor(10, 20);
  
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);
  
  display.display(); 
}

void alarm() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(300);
  }
}

void resetTimer() {
  running = false;
  studyMode = true;
  remainingTime = studyTime;

  digitalWrite(studyLED, LOW);
  digitalWrite(breakLED, LOW);
  digitalWrite(buzzer, LOW);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Timer Reset");
  display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Study Reminder");
  display.setCursor(0, 30);
  display.println("Press START");
  display.display();
}