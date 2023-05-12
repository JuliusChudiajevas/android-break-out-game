#pragma once
#include "SDL.h"

class TickTimer {
 private:
  Uint64 startTime;
  Uint64 timeOutMs;

 public:
  TickTimer(Uint64 timeOutMs);

  bool hasElapsed();
};