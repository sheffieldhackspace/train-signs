/*
  BigClock.h - Library for driving station BigClocks
*/
#ifndef BigClock_h
#define BigClock_h

#define BIG_CLOCK_WIDTH 96
#define BIG_CLOCK_HEIGHT 26

#include "Arduino.h"
#include "FreeRTOS.h"

#include "BigBoard.h"

class BigClock
{
  public:
    BigClock(byte *fb);
    void output();
    static void sCallback(void *arg);
    
  private:
    BigBoard *board[2];
};

#endif
