#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

Process::Process(int pid) : pid_{pid}, cpuUtilized_{CpuUtilization()} {}

// Returns this process's ID
int Process::Pid() { return pid_; }

// Returns this process's CPU utilization
float Process::CpuUtilization() {
  long currentActive{LinuxParser::ActiveJiffies(pid_)};
  long currentIdle{LinuxParser::IdleJiffies()};
  long currentTotal = currentActive + currentIdle;
  long deltaActive = currentActive - prevActive;
  long deltaTotal = currentTotal - prevTotal;
  cpuUtilized_ =
      static_cast<float>(deltaActive) / static_cast<float>(deltaTotal);
  prevActive = currentActive;
  prevTotal = currentTotal;
  return cpuUtilized_;
}

// Returns the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Returns this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Returns the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Returns the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Sorts processes according to their CPU Utilization
bool Process::operator<(Process const& a) const {
  return cpuUtilized_ > a.cpuUtilized_;
}
