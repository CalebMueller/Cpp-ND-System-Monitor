#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor() : prevActive{0}, prevTotal{0} {}
  float Utilization();

 private:
  long prevActive;
  long prevTotal;
};

#endif