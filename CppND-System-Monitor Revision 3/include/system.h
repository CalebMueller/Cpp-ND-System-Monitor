#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();  // TODO: See src/system.cpp
  vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  string Kernel();
  string OperatingSystem();
  vector<Process> processes_;

 private:
  Processor cpu_;
};

#endif