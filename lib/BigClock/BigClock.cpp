/*
  BigClock.h - Library for driving station BigClocks
 *
 * Copyright (c) 2013, Daniel Swann <hs@dswann.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the owner nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "BigClock.h"

BigClock::BigClock(byte *fb) {
  board[BOARD_TOP] = new BigBoard(fb, BOARD_TOP, D6, D4, D2);
  board[BOARD_BOTTOM] = new BigBoard(fb, BOARD_BOTTOM, D7, D5, D3);

  xTaskCreate(BigClock::sCallback, "keepalive", 4096, NULL, 2, NULL);
}

void BigClock::sCallback(void *arg) {
  pinMode(D10, OUTPUT);
  pinMode(D11, OUTPUT);

  while (true) {
    digitalWrite(D10, HIGH);
    digitalWrite(D11, HIGH);
    delay(2);
    digitalWrite(D10, LOW);
    digitalWrite(D11, LOW);
    delay(2);
  }
}

void BigClock::output() {
  board[BOARD_TOP]->output();
  board[BOARD_BOTTOM]->output();
}

