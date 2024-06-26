#include "MathGame.h"

void MathGame::changeCoord(int num, int &coord)
{
  while (num != 0)
  {
    coord++;
    num /= 10;
  }
}

void MathGame::displayProblem(LiquidCrystal_I2C &lcd, int &ans, int &coord, int &minPos)
{
  int r = random(4);
  int num1 = random(1, 11);

  int num2;
  if (r == 1)
  {
    num2 = num1;
    num1 = random(num2+1, 11);
  }
  if (r == 3)
  {
    num2 = num1;
    num1 = num2 * random(1, 11);
  }
  else
  {
    num2 = random(1, 11);
  }

  if (r == 1) {
    if (num1 < num2) {
      int temp = num1;
      num1 = num2;
      num2 = temp;
    }
  }

  lcd.clear();
  coord = 0;

  lcd.setCursor(0, 0);

  lcd.print(num1);
  changeCoord(num1, coord);

  lcd.print(" ");
  changeCoord(1, coord);

  lcd.print(op[r]);
  changeCoord(1, coord);

  lcd.print(" ");
  changeCoord(1, coord);

  lcd.print(num2);
  changeCoord(num2, coord);

  lcd.print(" = ");
  changeCoord(100, coord);

  minPos = coord;

  switch (r)
  {
  case 0:
    ans = num1 + num2;
    break;
  case 1:
    ans = num1 - num2;
    break;
  case 2:
    ans = num1 * num2;
    break;
  case 3:
    ans = num1 / num2;
    break;
  }
}

void MathGame::parseInput(LiquidCrystal_I2C &p1, LiquidCrystal_I2C &p2, int &ans, int &coord, int &minPos, int &pot, int &button, int &back, int &submit, int &input, bool &flag, bool &backFlag, bool &submitFlag, bool &solving, int &correct, Servo &player, bool &gameRunning, bool left, int &buzzer)
{
  if (!solving)
  {
    displayProblem(p1, ans, coord, minPos);
    solving = true;
  }

  int buttonVal = digitalRead(button);
  int backVal = digitalRead(back);
  int submitVal = digitalRead(submit);

  int num = map(analogRead(pot), 0, 1023, 0, 10);

  p1.setCursor(coord, 0);
  p1.print(num);
  p1.setCursor(coord, 0);

  if (backVal == 0)
  {
    if (backFlag)
    {
      if (coord > minPos)
      {
        p1.print(" ");
        coord--;
        p1.setCursor(coord, 0);
        input /= 10;
      }
      backFlag = false;
    }
  }
  else
    backFlag = true;

  if (buttonVal == 0)
  {
    if (flag)
    {
      coord++;
      input *= 10;
      input += num;

      p1.setCursor(coord, 0);

      flag = false;
    }
  }
  else
    flag = true;

  if (submitVal == 0)
  {
    if (submitFlag)
    {
      solving = false;
      input *= 10;
      input += num;
      p1.setCursor(0, 1);
      if (input == ans)
      {
        p1.print("Correct!");
        correct++;
        if (correct > threshold)
        {
          if (left) {
            player.write(140);
          }
          else {
            player.write(0);
          }
          p1.clear();
          p1.print("WINNER!");
          p2.clear();
          p2.print("LOSER!");
          gameRunning = false;
        }
      }
      else
      {
        p1.print("Wrong!");
      }
      input = 0;
      submitFlag = false;
    }
  }
  else
  {
    submitFlag = true;
  }
}

void MathGame::reset() {
  input1 = 0;
  input2 = 0;

  coord1 = 0;
  coord2 = 0;

  correct1 = 0;
  correct2 = 0;
  solving1 = false, solving2 = false;
}

void MathGame::playDeath(int &buzzer) {
  for (int thisNote = 0; thisNote < deathLength; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / deathNoteDurations[thisNote];

    tone(buzzer, deathMelody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:

    noTone(buzzer);

  }
}

void MathGame::runNextIteration(LiquidCrystal_I2C &lcd1, LiquidCrystal_I2C &lcd2, int &pot1, int &button1, int &back1, int &submit1, int &pot2, int &button2, int &back2, int &submit2, bool &flag1, bool &backFlag1, bool &submitFlag1, bool &flag2, bool &backFlag2, bool &submitFlag2, Servo &player, bool &gameRunning, int &buzzer)
{

  tone(buzzer, melody[(currentPlaying++) % melodyLength], 1000/noteDurations[currentPlaying % melodyLength]);
  delay(1000/noteDurations[(currentPlaying - 1) % melodyLength]);

  parseInput(lcd1, lcd2, ans1, coord1, minPos1, pot1, button1, back1, submit1, input1, flag1, backFlag1, submitFlag1, solving1, correct1, player, gameRunning, true, buzzer);

  if (!gameRunning)
  {
  noTone(buzzer);

    playDeath(buzzer);
    return;
  }
  parseInput(lcd2, lcd1, ans2, coord2, minPos2, pot2, button2, back2, submit2, input2, flag2, backFlag2, submitFlag2, solving2, correct2, player, gameRunning, false, buzzer);

  if (!gameRunning)
  {
  noTone(buzzer);

    playDeath(buzzer);
    return;
  }

  noTone(buzzer);


  // delay(250);
}