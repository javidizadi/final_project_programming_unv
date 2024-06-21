#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

struct DateTime {
  int year, month, day, hour, minutes, seconds;
};

void printDateTime(const DateTime &);
DateTime getCurrentLocalTime();
DateTime getUserDateTime();
bool validateDateTime(const DateTime &);

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

DateTime getUserDateTime() {

  DateTime result;

  cout << "Enter Year: ";
  cin >> result.year;
  cin.ignore();

  cout << "Enter month: ";
  cin >> result.month;
  cin.ignore();

  cout << "Enter day: ";
  cin >> result.day;
  cin.ignore();

  cout << "Enter hour: ";
  cin >> result.hour;
  cin.ignore();

  cout << "Enter minutes: ";
  cin >> result.minutes;
  cin.ignore();

  cout << "Enter Seconds: ";
  cin >> result.seconds;
  cin.ignore();

  return result;
}

bool validateDateTime(const DateTime &dt) {
  bool isValidDate = (dt.month <= 6 && dt.day <= 31) ||
                     (dt.month > 6 && dt.day <= 30) ||
                     (dt.month == 12 && dt.day <= 29);

  bool isValidTime = (dt.hour <= 24 && dt.minutes <= 60 && dt.seconds <= 60);

  return isValidTime && isValidDate;
}
