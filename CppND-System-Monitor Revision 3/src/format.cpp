#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long times) {
  using std::string;
  using std::to_string;

  // Since hrs is only 2 digits, if greater than 99
  // max out at 99
  string hrs = to_string((times / 3600));
  if (std::stoi(hrs) > 99) {
    hrs = "99";
  }
  string min = to_string((times / 60) % 60);
  string sec = to_string(times % 60);

  // pad leading zeroes in the case of single digits
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