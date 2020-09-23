#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;
using std::stof;
using std::to_string;

class Process {
 public:
  Process(int pid);  // constructor defined in process.cpp

  int Pid();               // Returns a processes identification number
  string User();           // Returns a processes associated user
  string Command();        // Returns a processes command
  string Ram();            // Returns the amount of ram process uses
  long int UpTime();       // Computes the up-time of a process
  float CpuUtilization();  // Computes the CPU utilization for process
  bool operator<(
      Process const& a) const;  // sorts processes based on CPU utilization

 private:
  int pid_;
  string user_;

  // CPU Utilization variables:
  float cpuUtilized_;
  long prevActive{0};  // previous active jiffies for the process
  long prevTotal{0};   // previous total jiffies for the system
};

#endif