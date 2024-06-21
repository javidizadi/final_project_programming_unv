#pragma once

#include <chrono>
#include <ctime>
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

DateTime getCurrentLocalTime() {
  time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
  tm *time = localtime(&t);

  return DateTime{time->tm_year, time->tm_mon, time->tm_yday,
                  time->tm_hour, time->tm_min, time->tm_sec};
}
