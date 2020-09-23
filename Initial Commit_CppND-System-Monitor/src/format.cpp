#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long times) {
  using std::string;
  using std::to_string;

  string hrs = to_string((times / 3600) %
                         100);  // hrs only 2 digits as per format instructions
  string min = to_string((times / 60) % 60);
  string sec = to_string(times % 60);

  if (hrs.length() < 2) {
    hrs = "0" + hrs;
  }
  if (min.length() < 2) {
    min = "0" + min;
  }
  if (sec.length() < 2) {
    sec = "0" + sec;
  }

  string formatTime = (hrs) + ":" + (min) + ":" + (sec);
  return formatTime;
}