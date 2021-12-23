#include <Arduino.h>

#define BUTTON_TOP WIO_KEY_C
#define BUTTON_MID WIO_KEY_B
#define BUTTON_BOT WIO_KEY_A

// remap buttons to suit orientation
#define JOYSTICK_UP WIO_5S_LEFT
#define JOYSTICK_DOWN WIO_5S_RIGHT
#define JOYSTICK_LEFT WIO_5S_DOWN
#define JOYSTICK_RIGHT WIO_5S_UP
#define JOYSTICK_PRESS WIO_5S_PRESS

volatile int buttonTopPressed = 0;
volatile int buttonMidPressed = 0;
volatile int joystickUpPressed = 0;
volatile int joystickDownPressed = 0;
volatile int joystickLeftPressed = 0;
volatile int joystickRightPressed = 0;
volatile int joystickPressPressed = 0;

void ISR_BUTTON_TOP();
void ISR_BUTTON_MID();
void ISR_JOYSTICK_UP();
void ISR_JOYSTICK_DOWN();
void ISR_JOYSTICK_LEFT();
void ISR_JOYSTICK_RIGHT();
void ISR_JOYSTICK_PRESS();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_TOP), ISR_BUTTON_TOP, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MID), ISR_BUTTON_MID, LOW);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_UP), ISR_JOYSTICK_UP, LOW);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_DOWN), ISR_JOYSTICK_DOWN, LOW);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_LEFT), ISR_JOYSTICK_LEFT, LOW);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_PRESS), ISR_JOYSTICK_PRESS, LOW);

  // Limitation: only WIO_5S_UP or WIO_KEY_A but not both
  //attachInterrupt(digitalPinToInterrupt(BUTTON_BOT), ISR_BUTTON_BOT, LOW);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_RIGHT), ISR_JOYSTICK_RIGHT, LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (buttonTopPressed)
  {
    Serial.print("buttonTopPressed\n");
    buttonTopPressed = 0;
  }

  if (buttonMidPressed)
  {
    Serial.print("buttonMidPressed\n");
    buttonMidPressed = 0;
  }

  if (digitalRead(WIO_KEY_A) == LOW)
  {
    Serial.print("buttonBotPressed\n");
  }

  if (joystickUpPressed)
  {
    Serial.print("joystickUpPressed\n");
    joystickUpPressed = 0;
  }

  if (joystickDownPressed)
  {
    Serial.print("joystickDownPressed\n");
    joystickDownPressed = 0;
  }

  if (joystickLeftPressed)
  {
    Serial.print("joystickLeftPressed\n");
    joystickLeftPressed = 0;
  }

  if (joystickRightPressed)
  {
    Serial.print("joystickRightPressed\n");
    joystickRightPressed = 0;
  }

  if (joystickPressPressed)
  {
    Serial.print("joystickPressPressed\n");
    joystickPressPressed = 0;
  }
}

void ISR_BUTTON_TOP() { buttonTopPressed = 1; }
void ISR_BUTTON_MID() { buttonMidPressed = 1; }
void ISR_JOYSTICK_UP() { joystickUpPressed = 1; }
void ISR_JOYSTICK_DOWN() { joystickDownPressed = 1; }
void ISR_JOYSTICK_LEFT() { joystickLeftPressed = 1; }
void ISR_JOYSTICK_RIGHT() { joystickRightPressed = 1; }
void ISR_JOYSTICK_PRESS() { joystickPressPressed = 1; }