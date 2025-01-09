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
int randomLED = 0;
int prev = 0;
int points = 0;
int misses = 0;


void blink_pattern(int &prev);
void blink_pointPattern();
void blink_random(int &prev);
void LEDoff();
void point(int &points);
void missed(int &misses);
void click_detected(int &prev, int &points, int &misses);
void win(int &cycle, int &randomLED, int &prev, int &points, int &misses);
void easyClear();
void printPointsAndMisses(int points, int misses);


void setup() {
  Serial.begin(9600);
  // Init
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




}

void loop() {
  Serial.println("BEFORE IFS");
  // Check per 20ms
  if (cycle % 2   == 0) {
    if (digitalRead(BUTTONPIN) == HIGH){
      click_detected(prev, points, misses);
    }
    if (points == 10){
      win(cycle, randomLED, prev, points, misses);
    }
  }


  // LED Updated every 200ms
  if (cycle % 20  == 0) {
    if (points >= 8){
      Serial.println("LED UPDATE");
      blink_random(prev);
    }

    else{
      blink_pattern(prev);
    }
  }


  // Every 5th second
  if (cycle % 100 == 0) {
    Serial.println("Stats");
    printPointsAndMisses(points, misses);

  }

  cycle++;
  Serial.println(cycle);

}







void blink_pattern(int &prev)
{
  int next = ++prev;


  if (next > 5) {
    next = 1;
  }

  switch (next)
  {
  case 1:
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, HIGH);
    prev = 1;
    break;

  case 2:
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, HIGH);
    prev = 2;
    break;

  case 3:
    digitalWrite(LEDPIN3, LOW);
    digitalWrite(LEDPIN4, HIGH);
    prev = 3;
    break;

  case 4:
    digitalWrite(LEDPIN4, LOW);
    digitalWrite(LEDPIN5, HIGH);
    prev = 4;
    break;

  case 5:
    digitalWrite(LEDPIN5, LOW);
    digitalWrite(LEDPIN1, HIGH);
    prev = 5;
    break;


  default:
    break;
  }
}




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




void blink_random(int &prev)
{
  int randLED = random(1, 6);

  while (randLED == prev)
  {
    randLED = random(1, 6);
  }


  digitalWrite(LEDPIN1, LOW);
  digitalWrite(LEDPIN2, LOW);
  digitalWrite(LEDPIN3, LOW);
  digitalWrite(LEDPIN4, LOW);
  digitalWrite(LEDPIN5, LOW);


  switch (randLED)
  {
  case 1:
    digitalWrite(LEDPIN1, HIGH);
    prev = 1;
    break;


  case 2:
    digitalWrite(LEDPIN2, HIGH);
    prev = 2;
    break;

  case 3:
    digitalWrite(LEDPIN3, HIGH);
    prev = 3;
    break;

  case 4:
    digitalWrite(LEDPIN4, HIGH);
    prev = 4;
    break;

  case 5:
    digitalWrite(LEDPIN5, HIGH);
    prev = 5;
    break;


  default:
    break;
  }

}



void LEDoff()
{
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, LOW);
    digitalWrite(LEDPIN4, LOW);
    digitalWrite(LEDPIN5, LOW);
}


void point(int &points)
{
  points++;

}

void missed(int &misses)
{
  misses++;

}

void click_detected(int &prev, int &points, int &misses)
{

    if (prev == 3) {
    blink_pointPattern();
    point(points);
    // PRINT POINTS TO DISPLAY HERE FUNCTION
  }

  else {
    LEDoff();
    missed(misses);
    delay(1000);
    // PRINT MISSED TO DISPLAY HERE
  }
}

void win(int &cycle, int &randomLED, int &prev, int &points, int &misses)
{

  easyClear();

  lcd.print("Congratulations!");
  lcd.setCursor(0, 1);
  lcd.print("    YOU WON!    ");
  delay(FIVESECONDS);

  easyClear();

  while (digitalRead(BUTTONPIN) == LOW)
  {
    printPointsAndMisses(points, misses);

    delay(FIVESECONDS);

    easyClear();

    lcd.print("Press the button");
    lcd.setCursor(0, 1);
    lcd.print(" To Play  Again ");
  }


}

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