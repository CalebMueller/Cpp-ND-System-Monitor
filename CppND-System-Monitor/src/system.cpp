#include "system.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"

#include <string>
#include <vector>

#include <iostream>

using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // empty old readings, make room for new readiings
  processes_.clear();
  // Get a vector of the current process ID's
  vector<int> pids{LinuxParser::Pids()};
  // Instantiate Process objects for the current processes
  for (int i = 0; i < static_cast<int>(pids.size()); i++) {
    int pid = pids[i];
    Process aprocess(pid);
    processes_.emplace_back(aprocess);
  }
  // sort the vector according to CPU Utilization
  sort(processes_.begin(), processes_.end());
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }