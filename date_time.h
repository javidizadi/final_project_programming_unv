#pragma once

#include <iomanip>
#include <iostream>

using namespace std;

struct DateTime {
  int year, month, day, hour, minutes, seconds;
};

void printDateTime(const DateTime &d) {
  cout << d.year << '-';
  cout << setfill('0') << setw(2) << d.month << '-';
  cout << setfill('0') << setw(2) << d.day;

  cout << '\t';

  cout << setfill('0') << setw(2) << d.hour << ':';
  cout << setfill('0') << setw(2) << d.minutes << ':';
  cout << setfill('0') << setw(2) << d.seconds;
}
