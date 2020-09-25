#include "linux_parser.h"

// Read and return operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return information about the cpu kernel
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memUtil;
  string line;
  string key;
  string value;
  string memTotal;
  string memFree;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        }
        if (key == "MemFree:") {
          memFree = value;
          memUtil = 1 - (stof(memFree) / stof(memTotal));
          return memUtil;
        }
      }
    }
  }
  return memUtil;
}

// Read and return the system uptime
long int LinuxParser::UpTime() {
  string line{0};
  string sysUpT{0};
  string sysIdleT{0};
  ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> sysUpT >> sysIdleT;
  }
  return static_cast<long>(stol(sysUpT));
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string entry;
  string pidDir = to_string(pid);
  ifstream filestream(kProcDirectory + pidDir + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    // burn off the first 21 entries of the line.
    // system up time is the 22nd token
    for (int i = 0; i < 22; i++) linestream >> entry;
  }
  long processStartTime = stol(entry);
  long systemUptime = UpTime();
  long processUpTime = processStartTime - systemUptime;
  return processUpTime;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string entry;
  vector<string> vJiff;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    while (linestream >> entry) {
      vJiff.push_back(entry);
    }
  }
  // vJiff=[0]cpu,
  // [1]user,[2]nice,[3]system,[4]idle,[5]iowait,[6]irq,[7]softirq,[8]steal,[9]guest,[10]guest_nice
  // acitveJiffies = user + nice + system + irq + softirq + steal
  long activeJiffies = stoi(vJiff[1]) + stoi(vJiff[2]) + stoi(vJiff[3]) +
                       stoi(vJiff[6]) + stoi(vJiff[7]) + stoi(vJiff[8]);
  return activeJiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string entry;
  vector<string> vJiff;
  string pidDir = to_string(pid);
  ifstream filestream(kProcDirectory + pidDir + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    // siphon off the first 17 entries of the line.
    // elements of interest for pid jiffies are 13 to 16
    while (linestream >> entry && vJiff.size() < 17) {
      vJiff.push_back(entry);
    }
  }
  //[13]utime, [14]stime, [15]cutime, [16]cstime
  long pidActiveJiffies =
      stoi(vJiff[13]) + stoi(vJiff[14]) + stoi(vJiff[15]) + stoi(vJiff[16]);
  return pidActiveJiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string entry;
  vector<string> vJiff;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    while (linestream >> entry) {
      vJiff.push_back(entry);
    }
  }
  // vJiff=[0]cpu,
  // [1]user,[2]nice,[3]system,[4]idle,[5]iowait,[6]irq,[7]softirq,[8]steal,[9]guest,[10]guest_nice
  // idleJiffies = idle + iowait
  long idleJiffies = stoi(vJiff[4]) + stoi(vJiff[5]);
  return idleJiffies;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalJiffies{0};
  string line;
  string entry;
  vector<string> vJiff;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    // line =
    // [0]cpuN,[1]user,[2]nice,[3]system,[4]idle,[5]iowait,[6]irq,[7]softirq,[8]steal,[9]guest,[10]guest_nice
    while (linestream >> entry) {
      vJiff.push_back(entry);
    }
  }
  for (int i = 1; i < 11; i++) {
    totalJiffies += stol(vJiff[i]);
  }
  return totalJiffies;
}

// Read and return a vector containing the cpu utilization for each running
// process
vector<string> LinuxParser::CpuUtilization() {
  // Vector container for computing CPU utilization
  // v_cpu = {recent_active, recent_total,last_active, last_total}
  // Once CpuUtilization() is called v_cpu cycles out an old reading for a
  // recent one Calculation of the float occurs in processor.cpp Utilization()
  static vector<string> v_cpu;
  // erase old readings
  if (v_cpu.size() > 2) {
    v_cpu.erase(v_cpu.end() - 2, v_cpu.end());
  }
  // insert new readings
  v_cpu.insert(v_cpu.begin(), to_string(Jiffies()));
  v_cpu.insert(v_cpu.begin(), to_string(ActiveJiffies()));
  return v_cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
      }
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return stoi(value);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
      }
      if (key == "procs_running") {
        return stoi(value);
      }
    }
  }
  return stoi(value);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string pidDir = to_string(pid);
  ifstream filestream(kProcDirectory + pidDir + kCmdlineFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
  }
  return line;
}

// Read and return the memory used by a process (in megabytes)
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  string pidDir = to_string(pid);
  ifstream filestream(kProcDirectory + pidDir + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          long sizeInMB = stol(value) / 1000;
          return to_string(sizeInMB);
        }
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  string pidDir = to_string(pid);
  ifstream filestream(kProcDirectory + pidDir + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid_of_pid = Uid(pid);
  string line;
  string name, pwd, uid;
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      // each line contains 7 tokens:
      // name:password:UID:GID:GECOS:directory:shell
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      linestream >> name >> pwd >> uid;
      if (uid == uid_of_pid) {
        return name;
      }
    }
  }
  return name;
}
