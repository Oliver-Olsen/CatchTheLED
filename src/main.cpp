//Oliver Olsen s205443
//Nils Wulff   s223968

// 12b
// 12c

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LEDPIN1 8
#define LEDPIN2 9
#define LEDPIN3 10
#define LEDPIN4 11
#define LEDPIN5 12
#define BUTTONPIN 13
#define ROW 0
#define COLUMN 0
#define FIVESECONDS 5000

LiquidCrystal_I2C lcd(0x27, 20, 4); // lcd address is 0x27, 16x2 lcd


int cycle = 0;
int prev = 1;
int points = 0;
int misses = 0;


void blink_pattern(int *prev);
void blink_pointPattern();
void blink_random(int *prev);
void LEDoff();
void setLED(int led);
void click_detected(int *prev, int *points, int *misses);
void win(int *cycle, int *prev, int *points, int *misses);
void easyClear();
void printPointsAndMisses(int points, int misses);


void setup() {
  Serial.begin(56700);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(ROW, COLUMN);


  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  pinMode(LEDPIN4, OUTPUT);
  pinMode(LEDPIN5, OUTPUT);
  pinMode(BUTTONPIN, INPUT);

  randomSeed(analogRead(A0));

  printPointsAndMisses(points, misses);




}

void loop() {
  // Check per 20ms
  if (digitalRead(BUTTONPIN) == HIGH){

    click_detected(&prev, &points, &misses);
    Serial.println("-");
    if (points == 10){
      win(&cycle, &prev, &points, &misses);
      }
    }
  


  // LED Updated every 200ms
  if (cycle >= 10) {
    cycle = 0;
    if (points >= 8){
      Serial.println("*");
      blink_random(&prev);
    }

    else{
      blink_pattern(&prev);
      Serial.println(".");
    }
  }




  cycle++;
  Serial.println(cycle);
  // 20 milisecond delay for button detection
  delay(20);

}






/**
 * @brief Blinks the LED's from left to right
 * 
 * @param prev Stores the previously lit LED
 */
void blink_pattern(int *prev)
{

  LEDoff();

  setLED(*prev);

  *prev += 1;

  if (*prev > 5) {
    *prev = 1;
  }
  
}



/**
 * @brief Lights the LED's in a pattern, when a point is scored
 * 
 */
void blink_pointPattern()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(LEDPIN3, HIGH);
    delay(50);
    digitalWrite(LEDPIN2, HIGH);
    digitalWrite(LEDPIN4, HIGH);
    delay(50);
    digitalWrite(LEDPIN1, HIGH);
    digitalWrite(LEDPIN5, HIGH);

    delay(50);
    LEDoff();

  }
}



/**
 * @brief Blinks in a random pattern. The same LED cannot be chonen twice
 * 
 * @param prev  Previously lit LED
 */
void blink_random(int *prev)
{
  int randLED = random(1, 6);

  do
  {
    randLED = random(1, 6);
  }
  while (randLED == *prev);

  *prev = randLED;

  LEDoff();
  setLED(*prev);

}


/**
 * @brief Turns off all 5 LED's
 * 
 */
void LEDoff()
{
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, LOW);
    digitalWrite(LEDPIN4, LOW);
    digitalWrite(LEDPIN5, LOW);
}


/**
 * @brief Used to light up the correct LED
 * 
 * @param led LED to be lit up
 */
void setLED(int led)
{
  switch (led)
  {
  case 1: digitalWrite(LEDPIN1, HIGH);  break;

  case 2: digitalWrite(LEDPIN2, HIGH);  break;

  case 3: digitalWrite(LEDPIN3, HIGH);  break;

  case 4: digitalWrite(LEDPIN4, HIGH);  break;

  case 5: digitalWrite(LEDPIN5, HIGH);  break;

  default:                              break;
  }
}



/**
 * @brief If a click is detected, a check will be run to verify if the 3rd LED was lit at the moment.
 * If the 3rd LED was lit, a point is added and the blink_patternPoints(), wil animate a point scored.
 * If the 3rd LED was not lit, the misses counter will be incremented, and the LED's will turn off for 1 second.
 * 
 * @param prev    Previously lit LED (which here is the current one)
 * @param points  Point counter
 * @param misses  Misses counter
 */
void click_detected(int *prev, int *points, int *misses)
{
  if (*prev == 3) {
    blink_pointPattern();
    *points += 1;
  }

  else {
    LEDoff();
    *misses += 1;
    delay(1000);
  }
  printPointsAndMisses(*points, *misses);
}


/**
 * @brief Winning message displayed then the points scored and total misses are displayed
 * 
 * 
 * @param cycle   Amount of time the loop function has lopped (resets at 10)
 * @param prev    Previously lit LED
 * @param points  Total Points
 * @param misses  Total Misses
 */
void win(int *cycle, int *prev, int *points, int *misses)
{

  easyClear();

  lcd.print("Congratulations!");
  lcd.setCursor(0, 1);
  lcd.print("    YOU WON!    ");
  
  delay(FIVESECONDS);

  easyClear();
  printPointsAndMisses(*points, *misses);

  delay(FIVESECONDS);

  easyClear();
  lcd.print("Press the button");
  lcd.setCursor(0, 1);
  lcd.print(" To Play  Again ");

  while (digitalRead(BUTTONPIN) == LOW) {};
  easyClear();
  *cycle = 0;
  *prev = 1;
  *points = 0;
  *misses = 0;
}



/**
 * @brief Clears everything currently displayed 16 x 2 (by printing spaces)
 * 
 */
void easyClear()
{
  // clears everything on the display
  for (int line = 0; line < 2; line++)
  {
    lcd.setCursor(0, line);
    lcd.print("                ");
  }
  lcd.setCursor(0, 0);
}


/**
 * @brief Prints the current points and misses
 * 
 * @param points Points Scored
 * @param misses Misses "Scored"
 */
void printPointsAndMisses(int points, int misses)
{
  lcd.setCursor(0, 0);
  lcd.print("Points:");
  lcd.setCursor(9, 0);
  lcd.print(points);
  lcd.setCursor(0, 1);
  lcd.print("Misses: ");
  lcd.setCursor(9, 1);
  lcd.print(misses);
}