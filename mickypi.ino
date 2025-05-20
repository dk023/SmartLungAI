#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo panServo;
Servo tiltServo;

int panAngle = 90;
int tiltAngle = 90;

int eyeX = 64, eyeY = 32; // Start in center
int pupilRadius = 5;

void drawEyes(int x, int y) {
  display.clearDisplay();
  display.drawCircle(32, 32, 15, SSD1306_WHITE);  // Left eye
  display.drawCircle(96, 32, 15, SSD1306_WHITE);  // Right eye

  display.fillCircle(32 + x, 32 + y, pupilRadius, SSD1306_WHITE); // Left pupil
  display.fillCircle(96 + x, 32 + y, pupilRadius, SSD1306_WHITE); // Right pupil

  display.display();
}

void setup() {
  Serial.begin(9600);

  panServo.attach(9);
  tiltServo.attach(10);
  panServo.write(panAngle);
  tiltServo.write(tiltAngle);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  if (Serial.available() >= 2) {
    int pan = Serial.read();
    int tilt = Serial.read();

    pan = constrain(pan, 30, 150);
    tilt = constrain(tilt, 70, 110);  // Reduced tilt range

    panServo.write(pan);
    tiltServo.write(tilt);

    // Map servo angle to eye movement
    int eyeOffsetX = map(pan, 30, 150, -5, 5);
    int eyeOffsetY = map(tilt, 70, 110, 5, -5); // Flip Y axis

    drawEyes(eyeOffsetX, eyeOffsetY);
  }
}
