#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

char readKeyPress();

void printMessage(string title, size_t xPadding = 5) {
  string body = string((title.length() + 2 * (xPadding + 1)), '=');
  cout << body << endl
       << '|' << string(xPadding, ' ') << title << string(xPadding, ' ') << '|'
       << endl
       << body << endl;
}

char getMenuChoice() {
  cout << endl << "Enter : ";

#ifdef _POSIX_C_SOURCE

  char choice = readKeyPress();

#else

  char choice;
  cin >> choice;
  cin.ignore();

#endif

  return choice;
}

#ifdef _POSIX_C_SOURCE
char readKeyPress() {
  fflush(stdout); // write everthing remains in stdout buffer

  char buf[2];

  FILE *fp = popen("read -s -n 1 choice && echo $choice", "r");
  if (fp == NULL) {
    cout << "Failed to read key press." << endl;
    exit(2);
  }

  fgets(buf, 2, fp);
  pclose(fp);

  return buf[0];
}
#endif
