#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Arduino.h>
#include "pitches.h"

class DinoGame {
  private:
    bool jumped1 = false;
    bool jumped2 = false;

    int gameSpeed1;
    int gameSpeed2;

    int waited1 = 0;
    int waited2 = 0;

    byte state1[16] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
    byte state2[16] = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

    int score1 = 0;
    int score2 = 0;

    int threshold = 200;

    int deathMelody[15] = {
  NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_C5, NOTE_E5, NOTE_G5,
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int deathNoteDurations[15] = {
  4, 4, 4,
  4, 4, 4,
  4, 4, 4,
  4, 4, 4,
  4, 4, 4,
};

int deathLength = 15;

  public:
    bool isCacti(byte val);
    bool isBird(byte val);
    void parseInput(LiquidCrystal_I2C &lcd, LiquidCrystal_I2C &p2, bool &jumped, int &pot, int &button, int &back, int &submit, bool &flag, bool &backFlag, bool &submitFlag, byte state[16], bool &gameRunning, Servo &player, bool left, int &buzzer, int &score);
    void runNextIteration(LiquidCrystal_I2C &lcd1, LiquidCrystal_I2C &lcd2, int &pot1, int &button1, int &back1, int &submit1, int &pot2, int &button2, int &back2, int &submit2, bool &flag1, bool &backFlag1, bool &submitFlag1, bool &flag2, bool &backFlag2, bool &submitFlag2, Servo &player, bool &gameRunning, int &buzzer);
  void playDeath(int &buzzer);
    void reset();
};