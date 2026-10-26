# Smart Study Break Reminder with Presence Detection

An Arduino-based smart study timer that tracks study and break intervals, featuring an ultrasonic sensor to detect whether the student is physically present at the desk. If the student leaves, the timer automatically pauses.

![Project Setup](images/setup.jpg) *(আপনার প্রজেক্টের একটি সুন্দর ছবি images ফোল্ডারে setup.jpg নামে সেভ করে গিটহাবে পুশ করুন)*

## 🎯 Project Purpose
Maintaining focus during long study sessions can be difficult. This project aims to implement the Pomodoro technique (customizable study and break times) with a hardware twist: it actively monitors the student's presence. If the student leaves the desk during a study session, the timer pauses and alerts them, ensuring productive study hours.

## 🛠️ Components Used
* **Microcontroller:** Arduino UNO Rev3
* **Display:** 1.3" I2C OLED Display (SH1106 Driver)
* **Sensor:** HC-SR04 Ultrasonic Distance Sensor
* **Indicators:** 1x Green LED (Study Mode), 1x Red LED (Break/Absent Mode), 1x Active Buzzer
* **Inputs:** 2x Push Buttons (Start & Reset)
* **Accessories:** Breadboard, 220Ω Resistors (x2), Jumper Wires (Male-to-Male & Male-to-Female), USB A to B Cable

## 🔌 Circuit Connections (Pin Mapping)

**1. OLED Display (I2C)**
* `VCC` ➔ Arduino `5V`
* `GND` ➔ Arduino `GND`
* `SDA` ➔ Arduino `A4`
* `SCL` ➔ Arduino `A5`

**2. HC-SR04 Ultrasonic Sensor**
* `VCC` ➔ Arduino `5V`
* `GND` ➔ Arduino `GND`
* `TRIG` ➔ Arduino `D6`
* `ECHO` ➔ Arduino `D7`

**3. LEDs & Buzzer**
* `Green LED (Positive)` ➔ Arduino `D9` (Negative via 220Ω resistor to GND)
* `Red LED (Positive)` ➔ Arduino `D10` (Negative via 220Ω resistor to GND)
* `Buzzer (Positive/Long Leg)` ➔ Arduino `D8` (Negative to GND)

**4. Push Buttons**
* `Start Button` ➔ One leg to Arduino `A0`, other leg to `GND`
* `Reset Button` ➔ One leg to Arduino `A1`, other leg to `GND`
*(Note: Internal INPUT_PULLUP is used in the code, so no external pull-up resistors are required).*

## 🚀 How to Run the Project
1. Clone this repository to your local machine.
2. Open the project folder using **VS Code** with the **PlatformIO** extension installed.
3. The required libraries (`Adafruit GFX` and `Adafruit SH110X`) will be automatically downloaded by PlatformIO via the `platformio.ini` file.
4. Connect your Arduino UNO to the PC.
5. Click the **Upload** button (right arrow icon) at the bottom taskbar of VS Code.
6. Press the **START** button on the breadboard to begin the study session!

## ⚙️ How It Works
* **Study Mode:** The timer runs as long as an object (the student) is detected within 70 cm of the ultrasonic sensor. The Green LED stays ON.
* **Absent Detection:** If the distance exceeds 70 cm, the Red LED turns ON, the timer pauses, and the OLED displays "Student Absent!".
* **Break Mode:** Once the study time ends, the buzzer beeps 3 times, the Red LED turns ON, and a 10-second break timer starts.