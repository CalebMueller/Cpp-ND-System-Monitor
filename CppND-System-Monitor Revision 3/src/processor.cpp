#include "processor.h"

// Calculates the cpu being utilized from a vector of active and total jiffies
float Processor::Utilization() {
  // get updated readings
  long currentActive = LinuxParser::ActiveJiffies();
  long currentIdle = LinuxParser::IdleJiffies();
  long currentTotal = currentActive + currentIdle;
  // obtain deltas
  long deltaActive = currentActive - prevActive;
  long deltaTotal = currentTotal - prevTotal;
  // calculate interpolation
  float beingUtilized =
      static_cast<float>(deltaActive) / static_cast<float>(deltaTotal);
  // active becomes previous
  prevActive = currentActive;
  prevTotal = currentTotal;

  return beingUtilized;
}