#include <Arduino.h>

#include "Free_Fonts.h" //include free fonts library from https://github.com/Seeed-Studio/Seeed_Arduino_LCD/blob/master/examples/320x240/Free_Font_Demo/Free_Fonts.h
#include "TFT_eSPI.h"
#include "RTC_SAMD51.h"

// Font example
#define FONT FMB12 // See "include/Free_Fonts.h" for options

// Variable updated inside interrupt service routine
volatile int joystickPressed = 0;

#define MAX_COLOURS 5
unsigned int colourBackground[MAX_COLOURS] = {TFT_RED, TFT_GREEN, TFT_YELLOW, TFT_WHITE, TFT_BLUE};
String colourWord[MAX_COLOURS]             = {"RED"  , "GREEN"  , "YELLOW"  , "WHITE"  , "BLUE"  };
unsigned int colourBackgroundIndex = 0;
unsigned int colourWordIndex = 1;
String headerFooter[2] = {"XXXXXXXX", "++++++++"};
unsigned int headerFooterIndex = 0;

#define LIVES_START_VALUE 3
unsigned int lives = LIVES_START_VALUE;
unsigned int score = 0;
unsigned int gameOver = 0;
unsigned int firstBoot = 1;
unsigned int previousUpdateSecondsTime = 0;

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer

RTC_SAMD51 rtc; // Real time clock

// Functions
void ISR_WIO_5S_PRESS();
void updateScoreAndLife();
void updateScreenFirstBoot();
void updateScreen();

// put your setup code here, to run once:
void setup()
{
  tft.begin();
  tft.setRotation(2);

  spr.createSprite(TFT_WIDTH, TFT_HEIGHT); // Create buffer (portrait)

  rtc.begin();
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  rtc.adjust(now);

  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIO_5S_PRESS), ISR_WIO_5S_PRESS, LOW);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

// put your main code here, to run repeatedly:
void loop()
{
  if (firstBoot)
  {
    updateScreenFirstBoot();

    // Wait till joystick pressed
    while (joystickPressed == 0) {}

    // Initialize variables
    joystickPressed = 0;
    firstBoot = 0;
    previousUpdateSecondsTime = rtc.now().secondstime();
  }

  if (rtc.now().secondstime() - previousUpdateSecondsTime > 0)    // Execute every 1 second
  {
    if (gameOver)
    {
      if (joystickPressed == 1)   // Restart game
      {
        score = 0;
        lives = LIVES_START_VALUE;
        gameOver = 0;

        // Set index to different initial values
        colourBackgroundIndex = 0;
        colourWordIndex = 1;

        joystickPressed = 0;
      }
    }
    else
    {
      updateScoreAndLife();
      updateScreen();
    }
    previousUpdateSecondsTime = rtc.now().secondstime();
  }
}

void ISR_WIO_5S_PRESS() { joystickPressed = 1; }

void updateScoreAndLife()
{
  if (colourBackgroundIndex == colourWordIndex)
  {
    if (joystickPressed == 1)
    {
      score += 1;
      joystickPressed = 0;
    }
    else
    {
      lives -= 1;
      if (lives == 0)
      {
        gameOver = 1;
      }
    }
  }
  else
  {
    if (joystickPressed == 1)
    {
      lives -= 1;
      if (lives == 0)
      {
        gameOver = 1;
      }
      joystickPressed = 0;
    }
  }
}

void updateScreenFirstBoot()
{
  spr.fillSprite(TFT_WHITE);

  spr.setTextDatum(MC_DATUM);
  spr.setFreeFont(FMB18);
  spr.setTextColor(TFT_BLACK);

  spr.drawString("Press", 120, 40);
  spr.drawString("joystick", 120, 80);
  spr.drawString("when", 120, 120);
  spr.drawString("word", 120, 160);
  spr.drawString("and", 120, 200);
  spr.drawString("colour", 120, 240);
  spr.drawString("matches", 120, 280);

  spr.pushSprite(0, 0); // Push to LCD
}

void updateScreen()
{
  colourBackgroundIndex = rand() % MAX_COLOURS;
  spr.fillSprite(colourBackground[colourBackgroundIndex]);

  spr.setTextDatum(TL_DATUM);
  spr.setFreeFont(FMB18);
  spr.setTextColor(TFT_BLACK);
  String scoreText = "Score:";
  scoreText = scoreText + String(score);
  spr.drawString(scoreText, 0, 0);
  String livesText = "Lives:";
  livesText = livesText + String(lives);
  spr.drawString(livesText, 0, 30);

  spr.setTextDatum(MC_DATUM);
  spr.setFreeFont(FMB24);
  spr.setTextColor(TFT_BLACK);

  spr.drawString(headerFooter[headerFooterIndex], 120, 110);
  spr.drawString(headerFooter[headerFooterIndex], 120, 250);
  if (gameOver)
  {
    spr.drawString("GAME", 120, 160);
    spr.drawString("OVER", 120, 200);

    spr.setFreeFont(FMB9);
    spr.drawString("Press joystick", 120, 280);
    spr.drawString("to try again", 120, 300);
  }
  else
  {
    colourWordIndex = rand() % MAX_COLOURS;
    spr.drawString(colourWord[colourWordIndex], 120, 180);
  }
  
  spr.pushSprite(0, 0); // Push to LCD

  headerFooterIndex = (headerFooterIndex + 1) % 2;
}