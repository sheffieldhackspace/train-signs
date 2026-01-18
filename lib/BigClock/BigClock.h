/*
  BigClock.h - Library for driving station BigClocks
*/
#ifndef BigClock_h
#define BigClock_h

#include "Arduino.h"

#include "BigBoard.h"

class BigClock
{
  public:
    BigClock();
    void init();
    void output(byte *fb);
    static void sCallback(void *arg);
    
  private:
    BigBoard *board[2];
};

#endif
